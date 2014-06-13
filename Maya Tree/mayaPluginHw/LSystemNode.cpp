//ADDED
#include "LSystemNode.h"
MPoint vec3ToMPoint(vec3 v);
//ADDED

MStatus returnStatus;

#define McheckErr(stat,msg)			\
	if ( MS::kSuccess != stat ) {	\
		cerr << msg;				\
		return MS::kFailure;		\
	}

MObject LSystemNode::time;
MObject LSystemNode::outputMesh;
MTypeId LSystemNode::id( 0x80000 );
//ADDED
MObject LSystemNode::step;
MObject LSystemNode::angle;
MObject LSystemNode::grammar;
//ADDED

void* LSystemNode::creator()
{
	return new LSystemNode;
}

MStatus LSystemNode::initialize()
{
	MFnUnitAttribute unitAttr;
	MFnTypedAttribute typedAttr;
	MFnNumericAttribute numericAttr;
	
	
	MStatus returnStatus;
	//ADDED
	step = numericAttr.create("step", "st", MFnNumericData::kDouble, returnStatus);
	McheckErr(returnStatus, "ERROR creating Node step attribute\n");

	angle = numericAttr.create("angle", "ang", MFnNumericData::kDouble, returnStatus);
	McheckErr(returnStatus, "ERROR creating Node angle attribute\n");

	LSystemNode::grammar = typedAttr.create( "grammar", "gram", MFnData::kString, &returnStatus );
	McheckErr(returnStatus, "ERROR creating grammar outputMesh attribute\n");
	//ADDED
	LSystemNode::time = unitAttr.create( "time", "tm",
										  MFnUnitAttribute::kTime,
										  0.0, &returnStatus );
	McheckErr(returnStatus, "ERROR creating Node time attribute\n");


	LSystemNode::outputMesh = typedAttr.create( "outputMesh", "out",
												 MFnData::kMesh,
												 &returnStatus ); 
	McheckErr(returnStatus, "ERROR creating Node output attribute\n");

	typedAttr.setStorable(false);

	returnStatus = addAttribute(LSystemNode::time);
	McheckErr(returnStatus, "ERROR adding time attribute\n");

	returnStatus = addAttribute(LSystemNode::outputMesh);
	McheckErr(returnStatus, "ERROR adding outputMesh attribute\n");

	//ADDED
	returnStatus = addAttribute(LSystemNode::step);
	McheckErr(returnStatus, "ERROR adding step attribute\n");

	returnStatus = addAttribute(LSystemNode::grammar);
	McheckErr(returnStatus, "ERROR adding grammar attribute\n");

	returnStatus = addAttribute(LSystemNode::angle);
	McheckErr(returnStatus, "ERROR adding angle attribute\n");
	//ADDED

	returnStatus = attributeAffects(LSystemNode::time,
								    LSystemNode::outputMesh);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");
	//ADDED
	returnStatus = attributeAffects(LSystemNode::step,
								    LSystemNode::outputMesh);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");
	returnStatus = attributeAffects(LSystemNode::angle,
								    LSystemNode::outputMesh);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");
	returnStatus = attributeAffects(LSystemNode::grammar,
								    LSystemNode::outputMesh);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");
	//ADDED

	return MS::kSuccess;
}

MObject LSystemNode::createMesh(const MTime& time, const double& step, const double& angle, const MString& grammar,
									MObject& outData, MStatus& stat)

{
	int				numVertices, frame;
	float			cubeSize;
	MFloatPointArray		points;
	MFnMesh			meshFS;


	//ADDED
	LSystem system;
	system.loadProgram(grammar.asChar());
	system.setDefaultAngle(angle);
    system.setDefaultStep(step);

	std::vector<LSystem::Branch> branches;
	system.process((int)time.value(), branches);

	MPointArray Lpoints;
	MIntArray faceCounts, faceConnects;
	for( int b = 0; b < branches.size(); b++)
	{
		MPoint m1, m2;
		vec3 branchUp1 = vec3(branches[b].first.n[0], branches[b].first.n[2], branches[b].first.n[1]); 
		vec3 branchUp2 = vec3(branches[b].second.n[0], branches[b].second.n[2], branches[b].second.n[1]); 
		m1 = vec3ToMPoint(branchUp1);
		m2 = vec3ToMPoint(branchUp2);
		
		CylinderMesh(m1, m2).appendToMesh(Lpoints, faceCounts, faceConnects);
	}
	//ADDED

	MObject newMesh = meshFS.create(Lpoints.length(), faceCounts.length(), Lpoints, faceCounts, faceConnects, outData, &stat);

	return newMesh;
}

MStatus LSystemNode::compute(const MPlug& plug, MDataBlock& data)

{
	MStatus returnStatus;

	if (plug == outputMesh) {
		/* Get time */
		MDataHandle timeData = data.inputValue( time, &returnStatus ); 
		McheckErr(returnStatus, "Error getting time data handle\n");
		MTime time = timeData.asTime();

		//ADDED
		MDataHandle stepData = data.inputValue( step, &returnStatus ); 
		McheckErr(returnStatus, "Error getting step data handle\n");
		double step = stepData.asDouble();
		MDataHandle angleData = data.inputValue( angle, &returnStatus ); 
		McheckErr(returnStatus, "Error getting angle data handle\n");
		double angle = angleData.asDouble();
		MDataHandle grammarData = data.inputValue( grammar, &returnStatus ); 
		McheckErr(returnStatus, "Error getting grammar data handle\n");
		MString grammar = grammarData.asString();
		//ADDED

		/* Get output object */

		MDataHandle outputHandle = data.outputValue(outputMesh, &returnStatus);
		McheckErr(returnStatus, "ERROR getting polygon data handle\n");

		MFnMeshData dataCreator;
		MObject newOutputData = dataCreator.create(&returnStatus);
		McheckErr(returnStatus, "ERROR creating outputData");

		createMesh(time, step, angle, grammar, newOutputData, returnStatus);
		McheckErr(returnStatus, "ERROR creating new Cube");

		outputHandle.set(newOutputData);
		data.setClean( plug );
	} else
		return MS::kUnknownParameter;

	return MS::kSuccess;
}


MPoint vec3ToMPoint(vec3 v)
{
	MPoint m;
	m[0] = v[0];
	m[1] = v[1];
	m[2] = v[2];
	return m;
}