//#include "abot.h"
#pragma warning( disable : 4786 )
#include "stdafx.h"
#include "item.h"
#include <stdio.h>
#include "properties.h"


void DebugItemBits(DWORD data, DWORD size, DWORD pos) 
{
	BYTE n;

	char DebugBuffer[200];
	sprintf(DebugBuffer,"%d: ",pos);
	if(size>32) size = 32;
	if(size!=32) data = data<<(32 - size);
	for(n = 0;n < size;n++) 
	{
		if((data & 0x80000000) !=0)
			sprintf(DebugBuffer,"%s1",DebugBuffer);
		else
			sprintf(DebugBuffer,"%s0",DebugBuffer);
		data = data<<1;
	}
	//D2Print(DebugBuffer);
}


BOOL CD2Item::ParseMods(BitFields &iPacket)
{
	int temp;
	mod_num=0;

	do
	{
		ITEM_MOD *p=&mod[mod_num];
		p->id=iPacket.GetField(9);

		if(mod_num>0)
		{
			if(p->id<mod[mod_num-1].id) 
			{
				mod_num=0;
				return FALSE;
			}
		}

		if(p->id==511) 
		{
			return TRUE;
		};

		D2_PROPERTIE* prop=GetPropertiesRow(p->id);
		if(!prop) break;

		for(int i=0;i<prop->numFields;i++)
		{
			p->value[i]=iPacket.GetField(prop->bits[i]);
			p->value[i]-=prop->bias;
		}
		switch(p->id)
		{
		case 48:
		case 50:
		case 52:
		case 54:
			p->num=1;
			p->value[0]+=p->value[1];
			break;
		case 57:
			p->num=2;
			//p->value[0]/=5;
			temp=(p->value[0]+p->value[1])/2;
			p->value[0]=p->value[2]*0.04;

			p->value[1]=temp*p->value[0]/10;

			break;
		default:
			p->num=prop->numFields;
			break;
		}
		
		mod_num++;
	}while(mod_num<MAX_MOD && iPacket.pos+9<iPacket.max);

	return FALSE;
}

BOOL CD2Item::FromPacket(BYTE* aPacket, DWORD aLen,BOOL identify)
{
	if (aPacket[0] == 0x9c)
		return Parse9C(aPacket, aLen);
	else if (aPacket[0] == 0x9d)
		return Parse9D(aPacket, aLen);
	return FALSE;
}

DWORD GetBitField(const BYTE* data, DWORD pos, DWORD len, DWORD max) 
{
	// trying to read beyond the end of the packet

	if(pos + len > max) {

		// if we are already at or beyond the end of packet, return 0;
		if(pos >= max) {
			return 0;
		}
		// otherwise adjust len to all remaining bits
		//	D2Print("Trying to read beyond eop. len:%d pos:%d max:%d",len,pos,max);
		len = max - pos;
	}

	return (DWORD)(*(unsigned __int64 *)(data+pos/8)<<(64-len-(pos&7))>>(64-len));
}

BOOL CD2Item::IsPotion()
{
	switch(code_id)
	{
	case 7108210: //rvl
	case 7566962: //rvl
	case 3240040: //hp1
	case 3305576: //hp2
	case 3371112: //hp3
	case 3436648: //hp4
	case 3502184: //hp5
	case 3240045: //mp1
	case 3305581: //mp2
	case 3371117: //mp3
	case 3436653: //mp4
	case 3502189: //mp5
	case  7565433:
		return TRUE;
	default: 
		return FALSE;
	}
}


//DEL void CD2Item::GetRandBits(BitFields &iPacket)
//DEL {
//DEL 	static int aaa=0;
//DEL 	D2Print("aaa=%d",aaa);
//DEL 	
//DEL 	for(int i=0;i<aaa;i++)
//DEL 		iPacket.GetField(1);
//DEL 	aaa++;
//DEL }

BOOL CD2Item::IsThrowItem()
{
	switch(code_id)
	{
	case 7758186: //jav
	case 6384185: //9ja
	case 6384183: //7ja

	case 7889268: //tax
	case 6386745: //9ta
	case 6386743: //7ta

	case 6712180: //tkf
	case 7042105: //9tk
	case 7042103: //7tk

	case 3501409: //am5
	case 6384993: //ama
	case 6712673: //amf

	case 7104880: //pil
	case 6910009: //9pi
	case 6910007: //7pi

	case 6712162: //bkf
	case 7037497: //9bk
	case 7037495: //7bk

	case 7102818: //bal
	case 3695161: //9b8
	case 3695159: //7b8

	case 7760999: //glv
	case 7104313: //9gl
	case 7104311: //7gl

	case 7369588: //tsp
	case 7566393: //9ts
	case 7566391: //7ts

		return TRUE;
	default: 
		return FALSE;
	}
}

char * CD2Item::GetColorCode()
{
	static char *cr_tb[10]={"","Inferior ","","Superior ","$$3","$$:","$$9","$$4","$$8","$$4Rune Word $$5"};
	static char *cr_tb_shocket[4]={"","$$5Inferior ","$$5","$$5Superior "};
	if(this->isRuneWord)
		return cr_tb[9];

	if(ItemQuality<4 && this->hasSockets)
		return cr_tb_shocket[ItemQuality];

	return cr_tb[ItemQuality];
}

int CD2Item::OperatorPriority(char op)
{
	switch(op)
	{
	case '*': return 5;
	case '/': return 5;
	case '+': return 4;
	case '-': return 4;
	case '=': return 3;
	case '#': return 3;
	case ']': return 3;
	case '[': return 3;
	case '>': return 3;
	case '<': return 3;
	case ',': return 2;
	case '|': return 1;
	case '(': return 0;
	case ')': return 0;
	default: return -1;
	}
}

void CD2Item::RemoveBlanks(char *str)
{
	char *p1,buf[1000];
	strcpy(buf,str);

	p1=buf;

	while(*p1 && p1-buf<sizeof(buf))
	{
		if(*p1==' '||*p1=='\t') 
		{
			p1++;
			continue;
		}
		
		*str=*p1;
		p1++;
		str++;
	}
	*str=0;
}

#define FILTER_UNFIT -1
#define FILTER_FAILED 0
#define FILTER_SUCCEEDED 1

const char* CD2Item::GetAToken(const char *p,SINGLE_TOKEN&token)
{
	token.type=TOKEN_TYPE_OP;

	switch(*p)
	{
	case 0: 
		token.type=TOKEN_TYPE_UNSET;
		return NULL;
	case ':': 
		token.op=']';
		return p+1;
	case '>': 
		if(p[1]=='=')
		{
			token.op=']';
			return p+2;
		}
		token.op='>';
		return p+1;
	case '<':
		if(p[1]=='=')
		{
			token.op='[';
			return p+2;
		}
		token.op='<';
		return p+1;
	case ',':
		token.op=',';
		return p+1;
	case '&': 
		if(p[1]=='&')
		{
			token.op=',';
			return p+2;;
		}
		break;
	case '!':
		if(p[1]=='=')
		{
			token.op='#';
			return p+2;;
		}
		break;
	case '|':
		token.op='|';
		if(p[1]=='|')
		{
			return p+2;;
		}
		return p+1;
	case '=': 
		token.op='=';
		if(p[1]=='=')
		{
			return p+2;;
		}
		return p+1;
	case ')': 
	case '(': 
	case '*': 
	case '/': 
	case '+': 
	case '-': 
		token.op=*p;
		return p+1;
	}

	if(*p>='A'&&*p<='Z' ||*p>='a'&&*p<='z')
	{
		//变量
		token.type=TOKEN_TYPE_VAR;
		char *pdst=token.var;
		while(*p>='A'&&*p<='Z' ||*p>='a'&&*p<='z'||*p>='0'&&*p<='9'||*p=='_'||*p=='%')
		{
			if(pdst-token.var<sizeof(token.var)-1)
			{
				*pdst=*p;
				pdst++;
			}

			p++;
		}
		*pdst=0;
		return p;
	}
	else if(*p>='0'&&*p<='9')
	{
		//变量
		token.type=TOKEN_TYPE_NUM;
		token.num=atoi(p);

		while(*p>='0'&&*p<='9')p++;
		return p;
	}
	else 
	{
//		D2Print("$$1Expression error: bad character '%c'",*p);
		token.type=TOKEN_TYPE_BAD;
		return p;
	}
}



BOOL CD2Item::Parse9C(const BYTE *aPacket, DWORD aLen)
{
	BitFields iPacket(aPacket,aLen);

	memset(this,0,sizeof(CD2Item));
	iPacket.GetField(8);//message
	action=(BYTE)iPacket.GetField(8);
	iPacket.GetField(8);//message size

	ItemType=(BYTE)iPacket.GetField(8);
//	DebugItemBits(iPacket.GetField(4),4,iPacket.pos);
	id=(DWORD)iPacket.GetField(32);

	this->isSocketsFull = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(3),3,iPacket.pos);
	this->isIdentified = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(1),1,iPacket.pos);
	this->isSwitchin = iPacket.GetField(1);
	this->isSwitchout = iPacket.GetField(1);

	this->isBroken = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(1),1,iPacket.pos);
	this->fromBelt = iPacket.GetField(1);
	this->hasSockets = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(1),1,iPacket.pos);
	this->isJustGenerated = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(2),2,iPacket.pos);


	this->isEar = iPacket.GetField(1);
	this->isStartitem = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(3),3,iPacket.pos);
	this->isMiscItem = iPacket.GetField(1);
	this->isEthereal = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(1),1,iPacket.pos);

	this->isPersonalized = iPacket.GetField(1);
	this->isGamble = iPacket.GetField(1);
	this->isRuneWord = iPacket.GetField(1);

	DebugItemBits(iPacket.GetField(18),18,iPacket.pos);

//	D2Print("%d[%d]",iPacket.pos/8, iPacket.pos%8);


	if(action== ITEM_ACTION_DROP||action== ITEM_ACTION_NEW_GROUND)
	{
		pos_x=(WORD)iPacket.GetField(16);
		pos_y=(WORD)iPacket.GetField(16);
	}
	else
	{
		DebugItemBits(iPacket.GetField(4),4,iPacket.pos);
		pos_x=(WORD)iPacket.GetField(4);
		pos_y=(WORD)iPacket.GetField(4);
		storage_id=(BYTE)iPacket.GetField(3);
	}

	if(isEar)return TRUE;

	code_id=iPacket.GetField(32);

	if(code[3]==' ') code[3]='\0';
	code[4]='\0';

	switch(code_id)
	{
	case 6581351://gld
		if(iPacket.GetField(1)) GoldAmount = iPacket.GetField(32);
		else GoldAmount = iPacket.GetField(12);
		isGold=TRUE;
		return TRUE;
	case 7037545:
	case 7037556:
	case 7955819:
	case 7892834://cube
	case 7762275://bolt
	case 7762273://arrow
		return TRUE;
	}
	
	// packet not long enough to read ItemLevel
	if(iPacket.pos+14>=iPacket.max) return TRUE;

	GemedNumber =(BYTE)iPacket.GetField(3);
	Ilvl = (BYTE)iPacket.GetField(7);
	ItemQuality = (BYTE)iPacket.GetField(4);

	BOOL flag1=iPacket.GetField(1);
	BOOL flag2=iPacket.GetField(1);

	if(flag1)//ring amu charm jew
	{
		DebugItemBits(iPacket.GetField(3),3,iPacket.pos);
	}
	else if(flag2)
	{
		DebugItemBits(iPacket.GetField(11),11,iPacket.pos);
	}

	int pos_begin=iPacket.pos;

///////////////////////////////////////////
	int retry_time=0;
RE_TRY:
	iPacket.pos=pos_begin;
///////////////////////////////////////////

	if(isIdentified)
	{
		switch(ItemQuality)
		{
		case ITEM_LEVEL_MAGIC:
			MagicPrefix = (WORD)iPacket.GetField(11);
			MagicSuffix = (WORD)iPacket.GetField(11);
			break;
		case ITEM_LEVEL_UNIQUE:
			UniqueId=(WORD)iPacket.GetField(12);
			break;
		case ITEM_LEVEL_SET:
			SetId=(WORD)iPacket.GetField(12);
			//GetRandBits(iPacket);
			//DebugItemBits(iPacket.GetField(5),5,iPacket.pos);
			break;
		case ITEM_LEVEL_SUPERIOR:
			//?
			DebugItemBits(iPacket.GetField(3),3,iPacket.pos);
			break;
		case ITEM_LEVEL_RARE:
		case ITEM_LEVEL_CRAFT:
			RarePrefix=(WORD)iPacket.GetField(8)-155;
			RareSuffix=(WORD)iPacket.GetField(8);
			if(!isIdentified) return TRUE;

			/////////////////////////////////////////////
			// 这一段不知道是什么含义，长度也不固定
			
			DebugItemBits(iPacket.GetField(6),6,iPacket.pos);
			const int rcskip[]={3,4,5,2,6,7,1,8,9};
			for(int i=0;i<rcskip[retry_time];i++)
			{
				DebugItemBits(iPacket.GetField(11),11,iPacket.pos);
			}
			/////////////////////////////////////////////
			break;
			
		}
	}

	if(this->isRuneWord)
	{
		DebugItemBits(iPacket.GetField(16),16,iPacket.pos);
	}

	//基本防御
	if(ItemType==ITEM_ITEMTYPE_ARMOR
		||ItemType==ITEM_ITEMTYPE_HELM
		||ItemType==ITEM_ITEMTYPE_SHIELD
		||ItemType==ITEM_ITEMTYPE_EXPANSION
		||ItemType==ITEM_ITEMTYPE_OTHER&&!flag1)
	{
		nDefense=iPacket.GetField(11)-10;
	}


	if(IsThrowItem())
	{
	//	GetRandBits(iPacket);
		//DebugItemBits(iPacket.GetField(52),52,iPacket.pos);
	//	nQuantity=iPacket.GetField(6);
		DebugItemBits(iPacket.GetField(9),9,iPacket.pos);
		DebugItemBits(iPacket.GetField(17),17,iPacket.pos);
	}
	else if( code_id==7496503 ) //幻化
	{
		DebugItemBits(iPacket.GetField(8),8,iPacket.pos);
	}
	else if(ItemType==ITEM_ITEMTYPE_BOW)
	{
		DebugItemBits(iPacket.GetField(17),17,iPacket.pos);
	}
	else if( !flag1)
	{
		nMaxDur=iPacket.GetField(8);
		nDur=iPacket.GetField(8);
		DebugItemBits(iPacket.GetField(1),1,iPacket.pos);
	}


	if( this->hasSockets)
	{
		nSocket=iPacket.GetField(4);
	}

	if(!isIdentified) return TRUE;

	if(ItemQuality==ITEM_LEVEL_SET)
	{
		DebugItemBits(iPacket.GetField(5),5,iPacket.pos);
	}

	BOOL bModOK=ParseMods(iPacket);

	///////////////////////////////////////////////////
	if(!bModOK)
	{
		if((ItemQuality==ITEM_LEVEL_RARE
			||ItemQuality==ITEM_LEVEL_CRAFT)
			&&retry_time<9)
		{
			nDefense=0;
			nMaxDur=0;
			nDur=0;
			nSocket=0;
			retry_time++;
			goto RE_TRY;
		}
	}
	///////////////////////////////////////////////////

	return TRUE;
}

BOOL CD2Item::Parse9D(const BYTE *aPacket, DWORD aLen)
{
	BitFields iPacket(aPacket,aLen);

	memset(this,0,sizeof(CD2Item));
	iPacket.GetField(8);//message
	action=(BYTE)iPacket.GetField(8);
	iPacket.GetField(8);//message size

	ItemType=(BYTE)iPacket.GetField(8);
//	DebugItemBits(iPacket.GetField(4),4,iPacket.pos);
	id=(DWORD)iPacket.GetField(32);

	iPacket.GetField(40); // for 0x9d

	this->isSocketsFull = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(3),3,iPacket.pos);
	this->isIdentified = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(1),1,iPacket.pos);
	this->isSwitchin = iPacket.GetField(1);
	this->isSwitchout = iPacket.GetField(1);

	this->isBroken = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(1),1,iPacket.pos);
	this->fromBelt = iPacket.GetField(1);
	this->hasSockets = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(1),1,iPacket.pos);
	this->isJustGenerated = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(2),2,iPacket.pos);


	this->isEar = iPacket.GetField(1);
	this->isStartitem = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(3),3,iPacket.pos);
	this->isMiscItem = iPacket.GetField(1);
	this->isEthereal = iPacket.GetField(1);
	DebugItemBits(iPacket.GetField(1),1,iPacket.pos);

	this->isPersonalized = iPacket.GetField(1);
	this->isGamble = iPacket.GetField(1);
	this->isRuneWord = iPacket.GetField(1);

	DebugItemBits(iPacket.GetField(18),18,iPacket.pos);

//	D2Print("%d[%d]",iPacket.pos/8, iPacket.pos%8);


	if(action== ITEM_ACTION_DROP||action== ITEM_ACTION_NEW_GROUND)
	{
		pos_x=(WORD)iPacket.GetField(16);
		pos_y=(WORD)iPacket.GetField(16);
	}
	else
	{
		DebugItemBits(iPacket.GetField(4),4,iPacket.pos);
		pos_x=(WORD)iPacket.GetField(4);
		pos_y=(WORD)iPacket.GetField(4);
		storage_id=(BYTE)iPacket.GetField(3);
	}

	if(isEar)return TRUE;

	code_id=iPacket.GetField(32);

	if(code[3]==' ') code[3]='\0';
	code[4]='\0';

	switch(code_id)
	{
	case 6581351://gld
		if(iPacket.GetField(1)) GoldAmount = iPacket.GetField(32);
		else GoldAmount = iPacket.GetField(12);
		isGold=TRUE;
		return TRUE;
	case 7037545:
	case 7037556:
	case 7955819:
	case 7892834://cube
	case 7762275://bolt
	case 7762273://arrow
		return TRUE;
	}
	
	// packet not long enough to read ItemLevel
	if(iPacket.pos+14>=iPacket.max) return TRUE;

	GemedNumber =(BYTE)iPacket.GetField(3);
	Ilvl = (BYTE)iPacket.GetField(7);
	ItemQuality = (BYTE)iPacket.GetField(4);

	BOOL flag1=iPacket.GetField(1);
	BOOL flag2=iPacket.GetField(1);

	if(flag1)//ring amu charm jew
	{
		DebugItemBits(iPacket.GetField(3),3,iPacket.pos);
	}
	else if(flag2)
	{
		DebugItemBits(iPacket.GetField(11),11,iPacket.pos);
	}

	int pos_begin=iPacket.pos;

///////////////////////////////////////////
	int retry_time=0;
RE_TRY:
	iPacket.pos=pos_begin;
///////////////////////////////////////////

	if(isIdentified)
	{
		switch(ItemQuality)
		{
		case ITEM_LEVEL_MAGIC:
			MagicPrefix = (WORD)iPacket.GetField(11);
			MagicSuffix = (WORD)iPacket.GetField(11);
			break;
		case ITEM_LEVEL_UNIQUE:
			UniqueId=(WORD)iPacket.GetField(12);
			break;
		case ITEM_LEVEL_SET:
			SetId=(WORD)iPacket.GetField(12);
			//GetRandBits(iPacket);
			//DebugItemBits(iPacket.GetField(5),5,iPacket.pos);
			break;
		case ITEM_LEVEL_SUPERIOR:
			//?
			DebugItemBits(iPacket.GetField(3),3,iPacket.pos);
			break;
		case ITEM_LEVEL_RARE:
		case ITEM_LEVEL_CRAFT:
			RarePrefix=(WORD)iPacket.GetField(8)-155;
			RareSuffix=(WORD)iPacket.GetField(8);
			if(!isIdentified) return TRUE;

			/////////////////////////////////////////////
			// 这一段不知道是什么含义，长度也不固定
			
			DebugItemBits(iPacket.GetField(6),6,iPacket.pos);
			const int rcskip[]={3,4,5,2,6,7,1,8,9};
			for(int i=0;i<rcskip[retry_time];i++)
			{
				DebugItemBits(iPacket.GetField(11),11,iPacket.pos);
			}
			/////////////////////////////////////////////
			break;
			
		}
	}

	if(this->isRuneWord)
	{
		DebugItemBits(iPacket.GetField(16),16,iPacket.pos);
	}

	//基本防御
	if(ItemType==ITEM_ITEMTYPE_ARMOR
		||ItemType==ITEM_ITEMTYPE_HELM
		||ItemType==ITEM_ITEMTYPE_SHIELD
		||ItemType==ITEM_ITEMTYPE_EXPANSION
		||ItemType==ITEM_ITEMTYPE_OTHER&&!flag1)
	{
		nDefense=iPacket.GetField(11)-10;
	}


	if(IsThrowItem())
	{
	//	GetRandBits(iPacket);
		//DebugItemBits(iPacket.GetField(52),52,iPacket.pos);
	//	nQuantity=iPacket.GetField(6);
		DebugItemBits(iPacket.GetField(9),9,iPacket.pos);
		DebugItemBits(iPacket.GetField(17),17,iPacket.pos);
	}
	else if( code_id==7496503 ) //幻化
	{
		DebugItemBits(iPacket.GetField(8),8,iPacket.pos);
	}
	else if(ItemType==ITEM_ITEMTYPE_BOW)
	{
		DebugItemBits(iPacket.GetField(17),17,iPacket.pos);
	}
	else if( !flag1)
	{
		nMaxDur=iPacket.GetField(8);
		nDur=iPacket.GetField(8);
		DebugItemBits(iPacket.GetField(1),1,iPacket.pos);
	}


	if( this->hasSockets)
	{
		nSocket=iPacket.GetField(4);
	}

	if(!isIdentified) return TRUE;

	if(ItemQuality==ITEM_LEVEL_SET)
	{
		DebugItemBits(iPacket.GetField(5),5,iPacket.pos);
	}

	BOOL bModOK=ParseMods(iPacket);

	///////////////////////////////////////////////////
	if(!bModOK)
	{
		if((ItemQuality==ITEM_LEVEL_RARE
			||ItemQuality==ITEM_LEVEL_CRAFT)
			&&retry_time<9)
		{
			nDefense=0;
			nMaxDur=0;
			nDur=0;
			nSocket=0;
			retry_time++;
			goto RE_TRY;
		}
	}
	///////////////////////////////////////////////////

	return TRUE;
}
