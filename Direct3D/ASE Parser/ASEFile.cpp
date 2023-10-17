#include "ASEfile.h"

#define	NUMBER_OF_RESERVEDWORD				233
LPSTR Token[256] =
{
	const_cast<LPSTR>("*3DSMAX_ASCIIEXPORT")		/*  0*/
,const_cast<LPSTR>("*COMMENT")		/*  1*/
,const_cast<LPSTR>("*SCENE")				/*  2*/
,const_cast<LPSTR>("*MATERIAL_LIST")				/*  3*/
,const_cast<LPSTR>("*MATERIAL")			/*  4*/
,const_cast<LPSTR>("*LIGHTOBJECT")				/*  5*/
,const_cast<LPSTR>("*GEOMOBJECT")				/*  6*/
,const_cast<LPSTR>("*NODE_NAME")				/*  7*/
,const_cast<LPSTR>("*NODE_TM")				/*  8*/
,const_cast<LPSTR>("*MESH")				/*  9*/
,const_cast<LPSTR>("*MESH_VERTEX")				/* 10*/
,const_cast<LPSTR>("*MESH_FACE_LIST")			/* 11*/
,const_cast<LPSTR>("*MESH_FACE")			/* 12*/
,const_cast<LPSTR>("*MESH_NUMTVERTEX")		/* 13*/
,const_cast<LPSTR>("*MESH_NUMCVERTEX")			/* 14*/
,const_cast<LPSTR>("*MESH_NORMALS")		/* 15*/
,const_cast<LPSTR>("*MESH_FACENORMAL")			/* 16*/
,const_cast<LPSTR>("*MESH_VERTEXNORMAL")		/* 17*/
,const_cast<LPSTR>("*HELPEROBJECT")		/* 18*/
,const_cast<LPSTR>("*NODE_PARENT")			/* 19*/
,const_cast<LPSTR>("*HELPER_CLASS")				/* 20*/
,const_cast<LPSTR>("*INHERIT_POS")			/* 21*/
,const_cast<LPSTR>("*INHERIT_ROT")				/* 22*/
,const_cast<LPSTR>("*INHERIT_SCL")				/* 23*/
,const_cast<LPSTR>("*TM_ROW0")				/* 24*/
,const_cast<LPSTR>("*TM_ROW1")					/* 25*/
,const_cast<LPSTR>("*TM_ROW2")					/* 26*/
,const_cast<LPSTR>("*TM_ROW3")					/* 27*/
,const_cast<LPSTR>("*TM_POS")				/* 28*/
,const_cast<LPSTR>("*TM_ROTAXIS")				/* 29*/
,const_cast<LPSTR>("*TM_ROTANGLE")			/* 30*/
,const_cast<LPSTR>("*TM_SCALE")				/* 31*/
,const_cast<LPSTR>("*TM_SCALEAXIS")			/* 32*/
,const_cast<LPSTR>("*TM_SCALEAXISANG")		/* 33*/
,const_cast<LPSTR>("*BOUNDINGBOX_MIN")			/* 34*/
,const_cast<LPSTR>("*BOUNDINGBOX_MAX")			/* 35*/
,const_cast<LPSTR>("*TIMEVALUE")		/* 36*/
,const_cast<LPSTR>("*MESH_NUMVERTEX")			/* 37*/
,const_cast<LPSTR>("*MESH_NUMFACES")			/* 38*/
,const_cast<LPSTR>("*GROUP")			/* 39*/
,const_cast<LPSTR>("*MESH_TVERT")				/* 40*/
,const_cast<LPSTR>("*MESH_TFACELIST")			/* 41*/
,const_cast<LPSTR>("*MESH_TVERTLIST")			/* 42*/
,const_cast<LPSTR>("*PROP_MOTIONBLUR")		/* 43*/
,const_cast<LPSTR>("*PROP_CASTSHADOW")			/* 44*/
,const_cast<LPSTR>("*PROP_RECVSHADOW")			/* 45*/
,const_cast<LPSTR>("*MATERIAL_REF")		/* 46*/
,const_cast<LPSTR>("*NODE_PARENT")			/* 47*/
,const_cast<LPSTR>("*SCENE_FILENAME")		/* 48*/
,const_cast<LPSTR>("*SCENE_FIRSTFRAME")		/* 49*/
,const_cast<LPSTR>("*SCENE_LASTFRAME")		/* 50*/
,const_cast<LPSTR>("*SCENE_FRAMESPEED")			/* 51*/
,const_cast<LPSTR>("*SCENE_TICKSPERFRAME")	/* 52*/
,const_cast<LPSTR>("*SCENE_ENVMAP")	/* 53*/
,const_cast<LPSTR>("*MAP_NAME")			/* 54*/
,const_cast<LPSTR>("*MAP_CLASS")					/* 55*/
,const_cast<LPSTR>("*MAP_SUBNO")				/* 56*/
,const_cast<LPSTR>("*MAP_AMOUNT")				/* 57*/
,const_cast<LPSTR>("*SCENE_AMBIENT_STATIC")	/* 58*/
,const_cast<LPSTR>("*MATERIAL_COUNT")	/* 59*/
,const_cast<LPSTR>("*MATERIAL_NAME")			/* 60*/
,const_cast<LPSTR>("*MATERIAL_CLASS")			/* 61*/
,const_cast<LPSTR>("*MATERIAL_AMBIENT")			/* 62*/
,const_cast<LPSTR>("*MATERIAL_DIFFUSE")		/* 63*/
,const_cast<LPSTR>("*MATERIAL_SPECULAR")			/* 64*/
,const_cast<LPSTR>("*MATERIAL_SHINE")		/* 65*/
,const_cast<LPSTR>("*MATERIAL_SHINESTRENGTH")	/* 66*/
,const_cast<LPSTR>("*MATERIAL_TRANSPARENCY")	/* 67*/
,const_cast<LPSTR>("*MATERIAL_WIRESIZE")	/* 68*/
,const_cast<LPSTR>("*MATERIAL_SHADING")		/* 69*/
,const_cast<LPSTR>("*MATERIAL_XP_FALLOFF")		/* 70*/
,const_cast<LPSTR>("*MATERIAL_SELFILLUM")		/* 71*/
,const_cast<LPSTR>("*MATERIAL_TWOSIDED")			/* 72*/
,const_cast<LPSTR>("*MATERIAL_FALLOFF")			/* 73*/
,const_cast<LPSTR>("*MATERIAL_XP_TYPE")		/* 74*/
,const_cast<LPSTR>("*MAP_DIFFUSE")		/* 75*/
,const_cast<LPSTR>("*MESH_VERTEX_LIST")			/* 76*/
,const_cast<LPSTR>("*MESH_NUMTVFACES")		/* 77*/
,const_cast<LPSTR>("*MESH_CVERTLIST")		/* 78*/
,const_cast<LPSTR>("*LIGHT_TYPE")			/* 79*/
,const_cast<LPSTR>("*LIGHT_SHADOWS")				/* 80*/
,const_cast<LPSTR>("*LIGHT_USELIGHT")			/* 81*/
,const_cast<LPSTR>("*LIGHT_SPOTSHAPE")			/* 82*/
,const_cast<LPSTR>("*LIGHT_USEGLOBAL")		/* 83*/
,const_cast<LPSTR>("*LIGHT_ABSMAPBIAS")			/* 84*/
,const_cast<LPSTR>("*LIGHT_OVERSHOOT")			/* 85*/
,const_cast<LPSTR>("*LIGHT_SETTINGS")			/* 86*/
,const_cast<LPSTR>("*LIGHT_COLOR")				/* 87*/
,const_cast<LPSTR>("*LIGHT_INTENS")				/* 88*/
,const_cast<LPSTR>("*LIGHT_ASPECT")			/* 89*/
,const_cast<LPSTR>("*LIGHT_TDIST")				/* 90*/
,const_cast<LPSTR>("*LIGHT_MAPBIAS")				/* 91*/
,const_cast<LPSTR>("*LIGHT_MAPRANGE")			/* 92*/
,const_cast<LPSTR>("*LIGHT_MAPSIZE")			/* 93*/
,const_cast<LPSTR>("*LIGHT_RAYBIAS")			/* 94*/
,const_cast<LPSTR>("*MESH_SMOOTHING")			/* 95*/
,const_cast<LPSTR>("*MESH_MTLID")			/* 96*/
,const_cast<LPSTR>("*WIREFRAME_COLOR")			/* 97*/
,const_cast<LPSTR>("*TM_ANIMATION")			/* 98*/
,const_cast<LPSTR>("*CONTROL_POS_TRACK")		/* 99*/
,const_cast<LPSTR>("*CONTROL_ROT_TRACK")		/*100*/
,const_cast<LPSTR>("*CONTROL_POS_SAMPLE")		/*101*/
,const_cast<LPSTR>("*CONTROL_ROT_SAMPLE")		/*102*/
,const_cast<LPSTR>("*CAMERAOBJECT")		/*103*/
,const_cast<LPSTR>("*CAMERA_TYPE")			/*104*/
,const_cast<LPSTR>("*CAMERA_SETTINGS")			/*105*/
,const_cast<LPSTR>("*CAMERA_NEAR")				/*106*/
,const_cast<LPSTR>("*CAMERA_FAR")				/*107*/
,const_cast<LPSTR>("*CAMERA_FOV")			/*108*/
,const_cast<LPSTR>("*CAMERA_TDIST")				/*109*/
,const_cast<LPSTR>("*NUMSUBMTLS")				/*110*/
,const_cast<LPSTR>("*SUBMATERIAL")			/*111*/
,const_cast<LPSTR>("*MAP_SPECULAR")				/*112*/
,const_cast<LPSTR>("*MAP_SHINE")			/*113*/
,const_cast<LPSTR>("*MAP_GENERIC")				/*114*/
,const_cast<LPSTR>("*BITMAP")				/*115*/
,const_cast<LPSTR>("*MAP_TYPE")				/*116*/
,const_cast<LPSTR>("*UVW_U_OFFSET")				/*117*/
,const_cast<LPSTR>("*UVW_V_OFFSET")			/*118*/
,const_cast<LPSTR>("*UVW_U_TILING")			/*119*/
,const_cast<LPSTR>("*UVW_V_TILING")			/*120*/
,const_cast<LPSTR>("*UVW_ANGLE")		/*121*/
,const_cast<LPSTR>("*UVW_BLUR")				/*122*/
,const_cast<LPSTR>("*UVW_BLUR_OFFSET")			/*123*/
,const_cast<LPSTR>("*UVW_NOUSE_AMT")	/*124*/
,const_cast<LPSTR>("*UVW_NOISE_SIZE")			/*125*/
,const_cast<LPSTR>("*UVW_NOISE_LEVEL")			/*126*/
,const_cast<LPSTR>("*UVW_NOISE_PHASE")			/*127*/
,const_cast<LPSTR>("*BITMAP_FILTER")	/*128*/
,const_cast<LPSTR>("*MESH_MAPPINGCHANNEL")	/*129*/
,const_cast<LPSTR>("*MESH_TFACE")		/*130*/
,const_cast<LPSTR>("*CONTROL_POS_BEZIER")		/*131*/
,const_cast<LPSTR>("*CONTROL_BEZIER_POS_KEY")	/*132*/
,const_cast<LPSTR>("*CONTROL_ROT_TCB")	/*133*/
,const_cast<LPSTR>("*CONTROL_TCB_ROT_KEY")		/*134*/
,const_cast<LPSTR>("*MAP_OPACITY")		/*135*/
,const_cast<LPSTR>("*MATERIAL_FACEMAP")	/*136*/
,const_cast<LPSTR>("*MESH_NUMSKINWEIGHT")		/*137*/
,const_cast<LPSTR>("*MESH_NUMBONE")				/*138*/
,const_cast<LPSTR>("*BONE_LIST")	/*139*/
,const_cast<LPSTR>("*BONE_NAME")			/*140*/
,const_cast<LPSTR>("*MESH_WVERTEXS")			/*141*/
,const_cast<LPSTR>("*MESH_WEIGHT")			/*142*/
,const_cast<LPSTR>("*BONE_BLENGING_WEIGHT")	/*143*/
,const_cast<LPSTR>("*MAP_REFLECT")	/*144*/
,const_cast<LPSTR>("*MAP_REFRACT")				/*145*/
,const_cast<LPSTR>("*MAP_BUMP")				/*146*/
,const_cast<LPSTR>("*SCENE_BACKGROUND_STATIC")	/*147*/
,const_cast<LPSTR>("*MAP_SELFILLUM")			/*148*/
,const_cast<LPSTR>("*LIGHT_EXCLUDELIST")		/*149*/
,const_cast<LPSTR>("*LIGHT_NUMEXCLUDED")	/*150*/
,const_cast<LPSTR>("*LIGHT_EXCLUDED_INCLUDE")/*151*/
,const_cast<LPSTR>("*LIGHT_EXCLUDED_AFFECT_ILLUM")	/*152*/
,const_cast<LPSTR>("*LIGHT_EXCLUDED_AFFECT_SHADOW")	/*153*/
,const_cast<LPSTR>("*LIGHT_EXCLUDED")			/*154*/
,const_cast<LPSTR>("*LIGHT_HOTSPOT")		/*155*/
,const_cast<LPSTR>("*LIGHT_FALLOFF")		/*156*/
,const_cast<LPSTR>("*MESH_FACEMAPLIST")			/*157*/
,const_cast<LPSTR>("*MESH_FACEMAP")		/*158*/
,const_cast<LPSTR>("*MESH_FACEMAPVERT")			/*159*/
,const_cast<LPSTR>("*BITMAP_INVERT")	/*160*/
,const_cast<LPSTR>("*SHAPEOBJECT")			/*161*/
,const_cast<LPSTR>("*SHAPE_LINECOUNT")			/*162*/
,const_cast<LPSTR>("*SHAPE_LINE")			/*163*/
,const_cast<LPSTR>("*SHAPE_VERTEXCOUNT")		/*164*/
,const_cast<LPSTR>("*SHAPE_VERTEX_KNOT")	/*165*/
,const_cast<LPSTR>("*SHAPE_VERTEX_INTERP")		/*166*/
,const_cast<LPSTR>("*SHAPE_CLOSED")		/*167*/
,const_cast<LPSTR>("*IK_JOINT")				/*168*/
,const_cast<LPSTR>("*IK_TYPE")				/*169*/
,const_cast<LPSTR>("*IK_DOF")				/*170*/
,const_cast<LPSTR>("*IK_XACTIVE")				/*171*/
,const_cast<LPSTR>("*IK_YACTIVE")				/*172*/
,const_cast<LPSTR>("*IK_ZACTIVE")				/*173*/
,const_cast<LPSTR>("*IK_XLIMITED")				/*174*/
,const_cast<LPSTR>("*IK_YLIMITED")				/*175*/
,const_cast<LPSTR>("*IK_ZLIMITED")				/*176*/
,const_cast<LPSTR>("*IK_XEASE")				/*177*/
,const_cast<LPSTR>("*IK_YEASE")				/*178*/
,const_cast<LPSTR>("*IK_ZEASE")				/*179*/
,const_cast<LPSTR>("*IK_LIMITEXACT")			/*180*/
,const_cast<LPSTR>("*IK_JOINTINFO")			/*181*/
,const_cast<LPSTR>("*LIGHT_ATTNSTART")			/*182*/
,const_cast<LPSTR>("*LIGHT_ATTNEND")		/*183*/
,const_cast<LPSTR>("*MAP_AMBIENT")				/*184*/
,const_cast<LPSTR>("*MESH_VERTCOL")				/*185*/
,const_cast<LPSTR>("*MESH_NUMCVFACES")			/*186*/
,const_cast<LPSTR>("*MESH_CFACELIST")			/*187*/
,const_cast<LPSTR>("*MESH_CFACE")			/*188*/

,const_cast<LPSTR>("*MAP_SHINESTRENGTH")		/*189*/
,const_cast<LPSTR>("*MAP_FILTERCOLOR")			/*190*/

,const_cast<LPSTR>("*NODE_VISIBILITY_TRACK")/*191*/
,const_cast<LPSTR>("*CONTROL_FLOAT_SAMPLE")	/*192*/
,const_cast<LPSTR>("*CONTROL_FLOAT_KEY")	/*193*/
 
,const_cast<LPSTR>("*BONE_PROPERTY")	/*194*/
,const_cast<LPSTR>("*BONE")			/*195*/
,const_cast<LPSTR>("*SKIN_INITTM")			/*196*/
,const_cast<LPSTR>("*CONTROL_SCALE_TRACK")/*197*/
,const_cast<LPSTR>("*CONTROL_SCALE_SAMPLE")		/*198*/
,const_cast<LPSTR>("*SCENE_AMBIENT_ANIM")	/*199*/
,const_cast<LPSTR>("*CONTROL_POINT3_KEY")		/*200*/
,const_cast<LPSTR>("*CONTROL_TCB_POINT3_KEY")	/*201*/
,const_cast<LPSTR>("*CONTROL_TCB_FLOAT_KEY")/*202*/
,const_cast<LPSTR>("*CONTROL_TCB_POS_KEY")		/*203*/
,const_cast<LPSTR>("*CONTROL_TCB_SCALE_KEY")	/*204*/
,const_cast<LPSTR>("*CONTROL_BEZIER_FLOAT_KEY")	/*205*/
,const_cast<LPSTR>("*CONTROL_BEZIER_POINT3_KEY")/*206*/
,const_cast<LPSTR>("*CONTROL_BEZIER_SCALE_KEY")	/*207*/
,const_cast<LPSTR>("*CONTROL_POS_LINEAR")	/*208*/
,const_cast<LPSTR>("*CONTROL_POS_TCB")		/*209*/
,const_cast<LPSTR>("*CONTROL_ROT_LINEAR")		/*210*/
,const_cast<LPSTR>("*CONTROL_ROT_BEZIER")		/*211*/
,const_cast<LPSTR>("*CONTROL_SCALE_LINEAR")		/*212*/
,const_cast<LPSTR>("*CONTROL_SCALE_TCB")	/*213*/
,const_cast<LPSTR>("*CONTROL_SCALE_BEZIER")		/*214*/
 
,const_cast<LPSTR>("*CONTROL_POS_KEY")			/*215*/
,const_cast<LPSTR>("*CONTROL_ROT_KEY")			/*216*/
,const_cast<LPSTR>("*CONTROL_SCALE_KEY")	/*217*/

,const_cast<LPSTR>("*CONTROL_POINT3_TCB")		/*218*/
,const_cast<LPSTR>("*CONTROL_POINT3_BEZIER")	/*219*/
,const_cast<LPSTR>("*CONTROL_COLOR_BEZIER")		/*220*/
,const_cast<LPSTR>("*CONTROL_POINT3_SAMPLE")	/*221*/

,const_cast<LPSTR>("*CONTROL_FLOAT_TCB")		/*222*/
,const_cast<LPSTR>("*CONTROL_FLOAT_BEZIER")	/*223*/
,const_cast<LPSTR>("*CONTROL_FLOAT_LINEAR")	/*224*/

,const_cast<LPSTR>("*CAMERA_ANIMATION")	/*225*/
,const_cast<LPSTR>("*LIGHT_ANIMATION")		/*226*/

,const_cast<LPSTR>("*MATERIAL_WIREUNITS")	/*227*/
 
,const_cast<LPSTR>("*SCENE_MESHFRAMESTEP")		/*228*/
,const_cast<LPSTR>("*SCENE_KEYFRAMESTEP")	/*229*/
 
,const_cast<LPSTR>("*PROP_HIDDEN")			/*230*/
,const_cast<LPSTR>("*PROP_NORENDERABLE")	/*231*/
,const_cast<LPSTR>("*PROP_BONE")		/*232*/
, };


ASE::CASELexer::CASELexer(LPSTR p_FileName, DWORD p_SizeOfBuffer) : CLexer(p_FileName, p_SizeOfBuffer)
{
}

ASE::CASELexer::~CASELexer()
{
}

//Lexical Analysis
LONG ASE::CASELexer::GetToken(LPSTR p_TokenString)
{
	// 1) 앞쪽 쓸데없는 문자찾기
	//    앞쪽에 공백이나 Tab이나 개행이나 캐리지리턴 같은 문자열이 있으면 무시한다.
	CHAR	Char = GetNextChar();
	while (Char == ' ' || Char == '\t' || Char == '\r' || Char == '\n')
	{
		Char = GetNextChar();
		if (!Char)
		{
			return	TOKEND_END;
		}
	}

	// 2) '{'인가를 확인한다.
	if (Char == '{')
	{
		p_TokenString[0] = '{';
		p_TokenString[1] = NULL;

		return	TOKEND_BLOCK_START;
	}

	// 3) '}'인가를 확인한다.
	else if (Char == '}')
	{
		p_TokenString[0] = '}';
		p_TokenString[1] = NULL;

		return	TOKEND_BLOCK_END;
	}
	// 4) '*'인가를 확인한다.
	//    만약 '*'로 시작하는 것은 Reserved word이므로 검색한다!
	else if (Char == '*')
	{
		LONG	p_StringPoint = 0;
		while (Char != ' ' && Char != '\t' && Char != '\r' && Char != '\n' && Char != '"' && Char != '{' && Char != '}')
		{
			p_TokenString[p_StringPoint] = Char;

			Char = GetNextChar();
			if (!Char)
			{
				return	TOKEND_END;
			}
			p_StringPoint++;
		}
		p_TokenString[p_StringPoint] = NULL;

		for (int i = 0; i < NUMBER_OF_RESERVEDWORD; i++)
		{
			if (!strcmp(p_TokenString, Token[i]))
			{
				// 맞는 Token을 찾았다! 그 번호를 return시켜준다.
				return	i;
			}
		}

		// 맞는 것은 전혀 찾지 못했다. 정의되지 않은 TOKEN이라고 돌려준다.
		return	TOKEND_NOTDEFINED;
	}
	// 5) 숫자인지를 확인한다.
	//    만약에 '0'에서 '9'사이이거나 '-'부하고 붙어 있으면 숫자라고 판단을 한다.
	else if (Char >= '0' && Char <= '9' || Char == '-')
	{
		LONG	p_StringPoint = 0;
		while ((Char >= '0' && Char <= '9') || Char == '-' || Char == '.')
		{
			p_TokenString[p_StringPoint] = Char;

			Char = GetNextChar();
			if (!Char)
			{
				return	TOKEND_END;
			}
			p_StringPoint++;
		}
		p_TokenString[p_StringPoint] = NULL;


		return	TOKEND_NUMBER;
	}
	// 6) String인지를 검사한다.
	// '"' 따음표로 시작하면 이것은 String인다!
	else if (Char == '"')
	{
		Char = GetNextChar();
		if (!Char)
		{
			return	TOKEND_END;
		}

		LONG	p_StringPoint = 0;
		while (Char != '"')
		{
			p_TokenString[p_StringPoint] = Char;

			Char = GetNextChar();
			if (!Char)
			{
				return	TOKEND_END;
			}
			p_StringPoint++;
		}
		p_TokenString[p_StringPoint] = NULL;

		return	TOKEND_STRING;
	}
	// 7) 지금까지 못찾았음 이건 허당이다~ 다음을 기약하자~
	else
	{
		// 첫글짜는 char이 보관하고 있으니 이것을 일단 p_TokenStirng에 넣고 다음 글자를 읽어 들인다.
		p_TokenString[0] = Char;
		Char = GetNextChar();
		if (!Char)
		{
			return	TOKEND_END;
		}

		// 한 단어를 p_TokeString에다가 복사를 한다.
		LONG	p_StringPoint = 1;
		while (Char != ' ' && Char != '\t' && Char != '\r' && Char != '\n')
		{
			p_TokenString[p_StringPoint] = Char;

			Char = GetNextChar();
			if (!Char)
			{
				return	TOKEND_END;
			}
			p_StringPoint++;
		}
		p_TokenString[p_StringPoint] = NULL;

		// 일반적인 Identifier라고 return한다.(Idendifier는 변수이름이라든지 뭐 그런거..)
		return	TOKEND_IDENTIFIER;
	}

	//p_TokenString[0]	 = NULL;

	//TRACE("Have no match token : %s\n", p_TokenString);
	//return TOKEND_NOTDEFINED;
}