#ifndef ybDataManager_H
#define ybDataManager_H
//==================Linux=================//
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>  
//===================C++==================//
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>
//=================GEANT4=================//
#include "globals.hh"
#include "G4String.hh"
#include "G4ios.hh"
//==================ROOT==================//
#include "TFile.h"
#include "TTree.h"
using namespace std;

class ybDataManager
{
public:
	static ybDataManager* GetInstance();
	
	void SetRunNum(int runid);
	void SetRootFileName(G4String name);
	void ResetFileMap();
	ofstream* GetFile(G4String filename);
	void CloseFile(G4String filename);
	void CloseAllFiles();
	void CreateRootFile();
private:
	ofstream* AddNewFile(const G4String filename);
	void GetBranchInfo(G4String& filename,G4String& brname,size_t& channel,G4String& type,G4String& brtype);

private:
	ybDataManager();
	~ybDataManager();
	
	class DataGarbo 
    {
	public:
		~DataGarbo()
        { 
            if (pDataMan!=NULL)
			{
                delete pDataMan;
			}
        }
    };
	
private:
	static ybDataManager *pDataMan;
    static DataGarbo fDataGarbo;
	G4String fTempFolder;
	G4String fRootFolder;
	G4String fRootFileName;
	G4String fPrefix;
	
	typedef map<G4String,ofstream*> ybFileMap;
	ybFileMap DataFileMap;
	
};


#endif

