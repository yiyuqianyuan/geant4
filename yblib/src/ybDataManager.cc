#include "ybDataManager.hh"

ybDataManager* ybDataManager::pDataMan = NULL;
ybDataManager::DataGarbo ybDataManager::fDataGarbo;

ybDataManager::ybDataManager()
{
	fTempFolder="temp";
	fRootFolder="data";
	fRootFileName="result";
	fPrefix="0_";
	if( opendir(fTempFolder.c_str())==NULL)
	{
		mkdir(fTempFolder,0777);
	}
	if( opendir(fRootFolder.c_str())==NULL)
	{
		mkdir(fRootFolder,0777);
	}
}

ybDataManager::~ybDataManager()
{
	for(ybFileMap::iterator pIter=DataFileMap.begin();pIter!=DataFileMap.end();pIter++)
	{
		delete pIter->second;
		pIter->second = NULL;
		DataFileMap.erase(pIter); 
	}
	G4cout<<"deleting ybDataManager ..."<<G4endl;
}

ybDataManager* ybDataManager::GetInstance()
{
	if(!pDataMan)
	{
		pDataMan = new ybDataManager;
	}
	return pDataMan;
}

void ybDataManager::SetRunNum(int runid)
{
	stringstream ss;
	ss<<runid;
	fPrefix=ss.str()+"_";
}

void ybDataManager::SetRootFileName(G4String name)
{
	size_t fPos=name.find(".root");
	if(fPos!=std::string::npos)
	{
		fRootFileName=name.remove(fPos,name.length()-fPos);
	}else
	{
		fRootFileName=name;
	}
}

void ybDataManager::ResetFileMap()
{
	DataFileMap.clear();
	ybFileMap().swap(DataFileMap); 
}


ofstream* ybDataManager::AddNewFile(const G4String name)
{
	G4String fTotalName=fTempFolder+"/"+name;
	ofstream *pNewFile=new ofstream(fTotalName,ios::app);
	DataFileMap.insert(make_pair(name,pNewFile)); 

	return pNewFile;
}

ofstream* ybDataManager::GetFile(G4String name)
{	
	G4String fFileName=fPrefix+name;
	ybFileMap::iterator pIter=DataFileMap.find(fFileName);
	if(pIter==DataFileMap.end())
	{
		return AddNewFile(fFileName);
	}else
	{
		return pIter->second;
	}
}

void ybDataManager::CloseFile(G4String name)
{
	G4String fFileName=fPrefix+name;
	ybFileMap::iterator pIter=DataFileMap.find(fFileName);
	if(pIter==DataFileMap.end())
	{
		G4cout<<"Error::ybDataManager!! file "<<fFileName<<" does not exist!"<<G4endl;
		exit(1);
	}else
	{
		if(pIter->second->is_open())
		{
			pIter->second->close();
		}
	}
}

void ybDataManager::CloseAllFiles()
{
	for(ybFileMap::iterator pIter=DataFileMap.begin();pIter!=DataFileMap.end();pIter++)
	{
		if(pIter->second->is_open())
		{
			pIter->second->close();
		}
	}

}

void ybDataManager::CreateRootFile()
{
	if(DataFileMap.empty())
	{
		return;
	}
	const size_t fDataFileNumber=DataFileMap.size();

	ifstream *fDataFile=new ifstream[fDataFileNumber];
	size_t *fChNum=new size_t[fDataFileNumber];
	G4String *fBranchName=new G4String[fDataFileNumber];
	G4String *fDataType=new G4String[fDataFileNumber];	
	G4String *fBranchType=new G4String[fDataFileNumber];
	void **fVariable=new void*[fDataFileNumber];

	G4String fTotalRootFileName=fRootFolder+"/"+fPrefix+fRootFileName+".root";
	TFile fRootFile(fTotalRootFileName,"recreate");
	TTree fTree("tree",fRootFileName);
	
	G4String fFileName;
	G4String fTotalDataFileName;
	ybFileMap::iterator pIter=DataFileMap.begin();
	for(size_t i=0;i<fDataFileNumber;i++,pIter++)
	{
		fFileName=pIter->first;
		fTotalDataFileName=fTempFolder+"/"+fFileName;
		fDataFile[i].open(fTotalDataFileName,ios::in);
		if(!fDataFile[i].is_open())
		{
			G4cout<<"Error::ybDataManager!! the infile '"<<fFileName<<"' fail to open !!!!!!!"<<G4endl;
			exit(1);
		}

		GetBranchInfo(fFileName,fBranchName[i],fChNum[i],fDataType[i],fBranchType[i]);
		if(fDataType[i]=="D")
		{
			fVariable[i]=new Double_t[fChNum[i]];
			fTree.Branch(fBranchName[i],(Double_t*)fVariable[i],fBranchType[i]);
		}else if(fDataType[i]=="I")
		{
			fVariable[i]=new Int_t[fChNum[i]];
			fTree.Branch(fBranchName[i],(Int_t*)fVariable[i],fBranchType[i]);
		}			
		
	}
	while(1)
	{
		for(size_t i=0;i<fDataFileNumber;i++)
		{
			if(fDataType[i]=="D")
			{
				for(size_t j=0;j<fChNum[i];j++)
				{
					fDataFile[i]>>((Double_t*)fVariable[i])[j];
				}
			}else if(fDataType[i]=="I")
			{
				for(size_t j=0;j<fChNum[i];j++)
				{
					fDataFile[i]>>((Int_t*)fVariable[i])[j];
				}
			}
		}
		if(fDataFile[0].eof())
			break;		
		fTree.Fill();				
	}
	fRootFile.cd();
	fTree.Write();
	fRootFile.Close();
	for(size_t i=0;i<fDataFileNumber;i++)
	{
		if(fDataType[i]=="D")
		{
			delete[] (Double_t*)fVariable[i];
		}else if(fDataType[i]=="I")
		{
			delete[] (Int_t*)fVariable[i];
		}
	}
	
	delete[] (Int_t**)fVariable;
	delete[] fDataFile;
	delete[] fChNum;
	delete[] fBranchName;
	delete[] fDataType;
	delete[] fBranchType;
}

//for example: 0_DSSD[40,20](D)_e.txt
//'0' is run-id ; 'DSSD' is detector name ; '40' is number of channels (X); '20' is number of channels (Y);
//'D' means data type is 'double' ; 'e' means 'energy'
//so,fChannelNumber=40+20, fBranchName="e_DSSD", fDataType="D", fBranchType="e_DSSD[60]/D"

void ybDataManager::GetBranchInfo(G4String& filename,G4String& brname,size_t& channel,G4String& type,G4String& brtype)
{
	G4String fDecName="",fChStr="",fType="",fVar="";
	size_t fCh=0;
	size_t leftpos=0; 
	size_t rightpos=0;
	G4String TmpStr="";
	//detector name
	TmpStr=filename;
	leftpos = filename.first('_');
	rightpos = filename.first('[');
	if(rightpos==std::string::npos)
	{
		rightpos = filename.first('(');
	}
	if(leftpos==std::string::npos || rightpos==std::string::npos)
	{
		G4cout<<"Error::ybDataManager!!  Extract BranchName from "<<filename<<G4endl;
		exit(1);
	}else
	{
		TmpStr.remove(rightpos,filename.length()-rightpos);
		TmpStr.remove(0,leftpos+1);
		fDecName=TmpStr;
	}	
	//channel number
	leftpos = filename.first('[');
	if(leftpos==std::string::npos)
	{
		fCh=1;
	}else
	{
		size_t midpos = filename.first(',');
		rightpos = filename.first(']');
		size_t numx=0,numy=0;
		TmpStr=filename;
		TmpStr.remove(midpos,filename.length()-midpos);
		TmpStr.remove(0,leftpos+1);		
		stringstream ss;
		ss.str("");
		ss<<TmpStr;
		ss>>numx;

		TmpStr=filename;
		TmpStr.remove(rightpos,filename.length()-rightpos);
		TmpStr.remove(0,midpos+1);	
		ss.clear();
		ss.str("");
		ss<<TmpStr;
		ss>>numy;
		fCh=numx+numy;
		
		ss.clear();
		ss.str("");
		ss<<fCh;
		ss>>fChStr; 
	}
	//data type
	TmpStr=filename;
	leftpos = filename.first('(');
	rightpos = filename.first(')');	
	if(leftpos==std::string::npos || rightpos==std::string::npos)
	{
		G4cout<<"Error::ybDataManager!!   Extract Data Type from "<<filename<<G4endl;
		exit(1);
	}else
	{
		TmpStr.remove(rightpos,filename.length()-rightpos);
		TmpStr.remove(0,leftpos+1);
		fType=TmpStr;
	}			
	//variable
	TmpStr=filename;
	leftpos = filename.last('_');
	rightpos = filename.first('.');	
	if(leftpos==std::string::npos || rightpos==std::string::npos)
	{
		G4cout<<"Error::ybDataManager!!   Extract Variable from "<<filename<<G4endl;
		exit(1);
	}else
	{
		TmpStr.remove(rightpos,filename.length()-rightpos);
		TmpStr.remove(0,leftpos+1);
		fVar=TmpStr;
	}	

	brname=fVar+"_"+fDecName;
	channel=fCh;
	type=fType;
	if(fCh==1)
	{
		brtype=fVar+"_"+fDecName+"/"+fType;
	}else
	{		
		brtype=fVar+"_"+fDecName+"["+fChStr+"]/"+fType;
	}
}
