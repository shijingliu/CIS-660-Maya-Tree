#include "LSystemCmd.h"
#include <maya/MGlobal.h>
#include <list>

#include "LSystem.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <maya/MPxCommand.h>
#include <string>
#include <maya/MArgList.h>

LSystemCmd::LSystemCmd() : MPxCommand()
{
	MStatus doIt( const MArgList& args );
}

LSystemCmd::~LSystemCmd()   
{
}

MStatus LSystemCmd::doIt( const MArgList& args)
{   
	MStatus stat = MS::kSuccess;                        
	LSystem system; 
	int Iterations = 2; 
	double stepSize = 1.0; 
	double degrees = 25.7f; 
	int NumTreeX = 1; 
	int NumTreeY = 1; 
	int Xdifference = 0; 
	int Ydifference = 0;
	int IterationDifference = 0; 
	string grammar = "F\nF->F[+F]F[-F]F";

    //loop through the command line arguments   
	for(int i = 0; i < args.length(); ++i)
	{
		//check for a valid arguments 
		if(args.asString(i, &stat) == "-stepSize")
		{
			MString para1 = args.asString(++i); 
			stepSize = para1.asDouble(); 
		}
		if(args.asString(i, &stat) == "-degrees")
		{
			MString para2 = args.asString(++i);
			degrees = para2.asDouble();
		}
		if(args.asString(i, &stat) == "-grammar")
		{
			MString para3 = args.asString(++i);
			grammar = para3.asChar();
		}
		if(args.asString(i, &stat) == "-iterations")
		{
			MString para4 = args.asString(++i);
			Iterations = para4.asInt(); 
		}
		if(args.asString(i, &stat) == "-numberX")
		{
			MString para5 = args.asString(++i);
			NumTreeX = para5.asInt(); 
		}
		if(args.asString(i, &stat) == "-numberY")
		{
			MString para6 = args.asString(++i);
			NumTreeY = para6.asInt(); 
		}
        if(args.asString(i, &stat) == "-Xdifference")
		{
			MString para7 = args.asString(++i);
			Xdifference = para7.asInt(); 
		}
		if(args.asString(i, &stat) == "-Ydifference")
		{
			MString para8 = args.asString(++i);
			Ydifference = para8.asInt(); 
		}
		if(args.asString(i, &stat) == "-IterationD")
		{
			MString para9 = args.asString(++i);
			IterationDifference = para9.asInt(); 
		}
	}

	system.loadProgramFromString(grammar);
    system.setDefaultAngle(degrees);
    system.setDefaultStep(stepSize);




	MString shader1 = MString("")+ "shadingNode -asShader phong -name phong1;"+ "sets -renderable true -noSurfaceShader true -empty -name phong1SG;"+"connectAttr -f phong1.outColor phong1SG.surfaceShader;"+"setAttr \"phong1.color\" -type double3 0 1 0;";
	MGlobal::executeCommand(shader1, true, true);  
	MString shader2 = MString("")+ "shadingNode -asShader phong -name phong2;"+ "sets -renderable true -noSurfaceShader true -empty -name phong2SG;"+"connectAttr -f phong2.outColor phong2SG.surfaceShader;"+"setAttr \"phong2.color\" -type double3 1 0 0;";
	MGlobal::executeCommand(shader2, true, true); 
	MString shader3 = MString("")+ "shadingNode -asShader phong -name phong3;"+ "sets -renderable true -noSurfaceShader true -empty -name phong3SG;"+"connectAttr -f phong3.outColor phong3SG.surfaceShader;"+"setAttr \"phong3.color\" -type double3 0 0 1;";
	MGlobal::executeCommand(shader3, true, true); 
	MString shader4 = MString("")+ "shadingNode -asShader phong -name phong4;"+ "sets -renderable true -noSurfaceShader true -empty -name phong4SG;"+"connectAttr -f phong4.outColor phong4SG.surfaceShader;"+"setAttr \"phong4.color\" -type double3 1 0 1;";
	MGlobal::executeCommand(shader4, true, true);
	MString shader5 = MString("")+ "shadingNode -asShader phong -name phong5;"+ "sets -renderable true -noSurfaceShader true -empty -name phong5SG;"+"connectAttr -f phong5.outColor phong5SG.surfaceShader;"+"setAttr \"phong5.color\" -type double3 1 1 0;";
	MGlobal::executeCommand(shader5, true, true); 
	MString shader6 = MString("")+ "shadingNode -asShader phong -name phong6;"+ "sets -renderable true -noSurfaceShader true -empty -name phong6SG;"+"connectAttr -f phong6.outColor phong6SG.surfaceShader;"+"setAttr \"phong6.color\" -type double3 0 1 1;";
	MGlobal::executeCommand(shader6, true, true); 
	for(int numoftreeX = 0; numoftreeX < NumTreeX; ++numoftreeX)
	{
		for(int numoftreeY = 0; numoftreeY < NumTreeY; ++numoftreeY)
		{
			int variationX = rand()%(2*Xdifference)-Xdifference; 
	        int variationY = rand()%(2*Ydifference)-Ydifference; 

			int IterD = rand()%(2*IterationDifference) - IterationDifference;
			for (int i = 0; i < (Iterations+IterD); i++)
			{
				std::string insn = system.getIteration(i);
				std::cout << insn << std::endl;

				std::vector<LSystem::Branch> branches;      
				system.process(i, branches);    

				for(int j = 0; j < branches.size(); j++)
				{

					//the six numbers for a curve   
					double firstX = branches.at(j).first.n[0] + (double)(numoftreeX*6.0)+variationX;
					double firstY = branches.at(j).first.n[1] + (double)(numoftreeY*6.0)+variationY;
					double firstZ = branches.at(j).first.n[2];
					double secondX = branches.at(j).second.n[0] + (double)(numoftreeX*6.0)+variationX;
					double secondY = branches.at(j).second.n[1] + (double)(numoftreeY*6.0)+variationY;
					double secondZ = branches.at(j).second.n[2]; 

					//generate the tree
					MString circle ("circle -ch on -o on -nr 0 1 0 -r 0.05 -name nurbsCircle1;"); 
					MString cylinder = circle + "string $initialCurve = (`curve -d 1 -p "+firstX+" "+firstZ+" "+firstY+" -p "+secondX+" "+secondZ+" "+secondY+" -k 0 -k 1`);"+"select -r nurbsCircle1 $initialCurve;"+ "string $cylinder1[] = (`extrude -ch true -rn false -po 1 -et 2 -ucp 1 -fpt 1 -upn 1 -rotation 0 -scale 1 -rsp 1 \"nurbsCircle1\" ($initialCurve)`);";
					
					int chooseNum = rand()%6+1; 

					MString cylinderShader = cylinder + "select -r $cylinder1[0];" + "sets -e -forceElement phong"+chooseNum+"SG;"; 
					MGlobal::executeCommand(cylinderShader, true, true);   
				}  
			}
		}
	}
	cout<<"Implement Me!"<<endl;    
    return MStatus::kSuccess;  
}    