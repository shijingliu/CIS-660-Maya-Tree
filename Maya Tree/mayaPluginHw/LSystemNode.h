#ifndef H_NODE
#define H_NODE

#include <maya/MTime.h>
#include <maya/MFnMesh.h>
#include <maya/MPoint.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>


#include <maya/MPxNode.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MFnMeshData.h>

#include <maya/MIOStream.h>

//ADDED
#include "LSystem.h"
#include "cylinder.h"
//ADDED

class LSystemNode : public MPxNode
{
public:
					LSystemNode() {};
	virtual 		~LSystemNode() {};
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);
	static  void*	creator();
	static  MStatus initialize();

	static MObject	time;
	static MObject	outputMesh;
	static MTypeId	id;
	//ADDED
	static MObject step;
	static MObject angle;
	static MObject grammar;
	//ADDED
protected:
	//MObject createMesh(const MTime& time, MObject& outData, MStatus& stat);
	MObject LSystemNode::createMesh(const MTime& time, const double& step, const double& angle, const MString& grammar,
									MObject& outData, MStatus& stat);
};

#endif