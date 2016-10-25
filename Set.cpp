// Set.cpp : implementation file
//

#include "stdafx.h"
#include "碗扣式满堂支架建模助手.h"
#include "Set.h"
#include "math.h"
#include "MyNode.h"
#include "xSkinButton.h"
#include "excel.h"
//#include "msword.h"
#include "WordOffice.h"
#include "UIThread.h"
#include "Section.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <math.h>
#include <string>

using namespace std;


struct threadInfo
{
	int Process;
	CProgressCtrl* pctrlProgress;
	//CWnd* pCWnd;
};
struct fline
{
	float xs;
	float ys;
	float zs;
	float xe;
	float ye;
	float ze;
};
struct Node 
{
	float x;
	float y;
	float z;
	float YuanXin_X;
	float YuanXin_Y;
	int Num;
};

struct element
{
	int qd;
	int zd;
	//int unit;//1,2,3,4,5,6分别代表x向y向z向杆件及xy面xz面yz面剪刀撑
	float xs;
	float ys;
	float zs;
	float xe;
	float ye;
	float ze;
};
//UINT ThreadFunc(LPVOID lpParam);
threadInfo Info;
int Process=0;
UINT ThreadFunc(LPVOID lpParam)
{
	threadInfo* pInfo=(threadInfo*)lpParam;
	pInfo->pctrlProgress->SetPos(Process);
	return 0;
};

CxSkinButton m_button_Output;
CxSkinButton m_button_Clear;

float PaiJuDealSave[50];
int PaiJuCountSave[50];
float PaiJuDataSave[50];


float ZhuJuDealSave[50];
int ZhuJuCountSave[50];
float ZhuJuDataSave[50];

float BuJuDealSave[50];
int BuJuCountSave[50];
float BuJuDataSave[50];


int Sort=0;
int PaiJuCount=0;
int ZhuJuGeShu_X=0;
int BuJuGeShu_Z=0;
int PaiJuGeShu_Y=0;
int NodeZongShu=0;
float Length_X=0;
float Length_Y=0;
float Length_Z=0;

//***************************荷载参数初始值保存变量*****************//
float ini_GangJinHunNingTu=0.0;
float ini_MoBanFangLeng=0.0;
float ini_FuJiaGouJian=0.0;
float ini_RenChaiJi=0.0;
float ini_JiaoZhuZhenDao=0.0;
float ini_FengHeZai=0.0;




//************************修改VB程序设定的变量************************//
int a,c;
int b=0;
int dy=1;               //单元编号
int dc=0      ;            //单层节点数
int qd=  0      ;           //单元前节点
int zd=  0      ;           //单元后节点

//************************修改VB程序斜向单元设定的变量****************************//

int J1=1;
int j2=1;
int i1=1;
int i2=1;
int is1=0;
int s1=1;
int s2=1;
int k1=0;
int k2=0;
int k3=0;
int k4=0;

//************************循环变量设定*************************//
int xx=0;     //专业循环变量
int yy=0;     //专业循环变量
int zz=0;     //专业循环变量
int rr=0; 
int tt=0;
int kk=0;
int yj1=0;
int jy1=0;
int hh=0;
int ee=0;
int ff=0;
int cc=0;
int xi1=0;

//************************荷载参数************************//
int m1=0;
int m2=0;
int m3=0;
int u=0;
float qy=0;
float qq6, qq5, qq4, qq3, qq2, qj1, qj2, qy1, qy2, qy3;

Node *NodeZong;
////////////////////以下为office服务宏定义/////////////////////////////////////
#define xlAscending (long) 1

#define xlDescending (long) 2

#define vOpt COleVariant((long) DISP_E_PARAMNOTFOUND, VT_ERROR)

#define xlHeader (long) 1                            // 选取的区域有标题

#define xlNoHeader (long) 2                          // 选取的区域无标题    一定要正确确定是否有标题，否则排序可能不成功

#define xlMatchCase COleVariant((long) 1)

#define xlIgnoreCase COleVariant((long) 0)

#define xlTopToBottom (long) 1                       // 垂直方向进行排序

#define xlLeftToRight (long) 2                       // 水平方向进行排序

#define xlPinYin (long) 1 // this is the default     // 按字符的中文拼音进行排序

#define xlStroke (long) 2                            // 按每个字符中的笔画数进行排序
////////////////////以上为office服务宏定义/////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSet dialog


CSet::CSet(CWnd* pParent /*=NULL*/)
: CDialog(CSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSet)
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_DiErCiJiaoZhu = _T("");
	m_DiErCiYuYa = _T("");
	m_SaoDiGanGaoDu = _T("");
	m_DingCengXuanBi = _T("");
	m_DiSanCiYuYa = _T("");
	m_DiYiCiJiaoZhu = _T("");
	m_DiYiCiYuYa = _T("");
	m_FengHeZai = _T("");
	m_FuJiaGouJian = _T("");
	m_GangJinHunNingTu = _T("");
	m_JiaoZhuZhenDao = _T("");
	m_MoBanFangLeng = _T("");
	m_PaiJu_Y = _T("");
	m_RenChaiJi = _T("");
	m_BujuZ = _T("");
	m_ZhuJuX = _T("");
	//}}AFX_DATA_INIT
	//EnableVisualManagerStyle(TRUE, TRUE);
}


void CSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSet)
	DDX_Control(pDX, IDC_EDIT_XLSPATH, m_Path);
	DDX_Control(pDX, IDXLCD_DanWei_RenChaiJi, m_Control_DanWei_RenChaiJi);
	DDX_Control(pDX, IDXLCD_DanWei_MoBanFangLeng, m_Control_DanWei_MoBanFangLeng);
	DDX_Control(pDX, IDXLCD_DanWei_JiaoZhuZhenDao, m_Control_DanWei_JiaoZhuZhenDao);
	DDX_Control(pDX, IDXLCD_DanWei_GangJinHunNingTu, m_Control_DanWei_GangJinHunNingTu);
	DDX_Control(pDX, IDXLCD_DanWei_FuJiaGouJian, m_Control_DanWei_FuJiaGouJian);
	DDX_Control(pDX, IDXLCD_DanWei_FengHeZai, m_Control_DanWei_FengHeZai);
	DDX_Control(pDX, IDC_ZhuJu_X, m_Control_ZhuJu_X);
	DDX_Control(pDX, IDC_BuJu_Z, m_Control_BuJu_Z);
	DDX_Text(pDX, IDE_DiErCiJiaoZhu, m_DiErCiJiaoZhu);
	DDX_Text(pDX, IDE_DiErCiYuYa, m_DiErCiYuYa);
	DDX_Text(pDX, IDE_SaoDiGanGaoDu, m_SaoDiGanGaoDu);
	DDX_Text(pDX, IDE_DingCengXuanBi, m_DingCengXuanBi);
	DDX_Text(pDX, IDE_DiSanCiYuYa, m_DiSanCiYuYa);
	DDX_Text(pDX, IDE_DiYiCiJiaoZhu, m_DiYiCiJiaoZhu);
	DDX_Text(pDX, IDE_DiYiCiYuYa, m_DiYiCiYuYa);
	DDX_Text(pDX, IDE_FengHeZai, m_FengHeZai);
	DDX_Text(pDX, IDE_FuJiaGouJian, m_FuJiaGouJian);
	DDX_Text(pDX, IDE_GangJinHunNingTu, m_GangJinHunNingTu);
	DDX_Text(pDX, IDE_JiaoZhuZhenDao, m_JiaoZhuZhenDao);
	DDX_Text(pDX, IDE_MoBanFangLeng, m_MoBanFangLeng);
	DDX_Text(pDX, IDE_PaiJu_Y, m_PaiJu_Y);
	DDX_Text(pDX, IDE_RenChaiJi, m_RenChaiJi);
	DDX_Text(pDX, IDE_BuJuZ, m_BujuZ);
	DDX_Text(pDX, IDE_ZhuJuX, m_ZhuJuX);
	//DDX_Control(pDX,IDB_Output,m_button_Output);//田海涛
	//DDX_Control(pDX,IDB_Clear,m_button_Clear);//田海涛
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSet, CDialog)
//{{AFX_MSG_MAP(CSet)
ON_CBN_SELCHANGE(IDC_BuJu_Z, OnSelchangeBuJuZ)
ON_CBN_SELCHANGE(IDC_ZhuJu_X, OnSelchangeZhuJuX)
ON_CBN_SELCHANGE(IDXLCD_DanWei_FengHeZai, OnSelchangeDanWeiFengHeZai)
ON_CBN_SELCHANGE(IDXLCD_DanWei_FuJiaGouJian, OnSelchangeDanWeiFuJiaGouJian)
ON_CBN_SELCHANGE(IDXLCD_DanWei_GangJinHunNingTu, OnSelchangeDanWeiGangJinHunNingTu)
ON_CBN_SELCHANGE(IDXLCD_DanWei_JiaoZhuZhenDao, OnSelchangeDanWeiJiaoZhuZhenDao)
ON_CBN_SELCHANGE(IDXLCD_DanWei_MoBanFangLeng, OnSelchangeDanWeiMoBanFangLeng)
ON_CBN_SELCHANGE(IDXLCD_DanWei_RenChaiJi, OnSelchangeDanWeiRenChaiJi)
ON_EN_CHANGE(IDE_GangJinHunNingTu, OnChangeGangJinHunNingTu)
ON_EN_CHANGE(IDE_JiaoZhuZhenDao, OnChangeJiaoZhuZhenDao)
ON_EN_CHANGE(IDE_MoBanFangLeng, OnChangeMoBanFangLeng)
ON_EN_CHANGE(IDE_FuJiaGouJian, OnChangeFuJiaGouJian)
ON_EN_CHANGE(IDE_RenChaiJi, OnChangeRenChaiJi)
ON_EN_CHANGE(IDE_FengHeZai, OnChangeFengHeZai)
ON_EN_CHANGE(IDE_DiYiCiJiaoZhu, OnChangeDiYiCiJiaoZhu)
ON_EN_CHANGE(IDE_DiErCiYuYa, OnChangeDiErCiYuYa)
ON_EN_CHANGE(IDE_DiYiCiYuYa, OnChangeDiYiCiYuYa)
ON_EN_CHANGE(IDE_DiSanCiYuYa, OnChangeDiSanCiYuYa)
ON_EN_CHANGE(IDE_DiErCiJiaoZhu, OnChangeDiErCiJiaoZhu)
ON_BN_CLICKED(IDB_test, Ontest)
ON_EN_CHANGE(IDE_ZhuJuX, OnChangeZhuJuX)
ON_EN_CHANGE(IDE_BuJuZ, OnChangeBuJuZ)
ON_EN_CHANGE(IDE_SaoDiGanGaoDu, OnChangeSaoDiGanGaoDu)
ON_EN_CHANGE(IDE_DingCengXuanBi, OnChangeDingCengXuanBi)
ON_BN_CLICKED(IDB_Output, OnOutput)
ON_BN_CLICKED(IDB_Clear, OnClear)
ON_BN_CLICKED(IDC_BUTTON_INPUTEXCEL, OnButtonInputexcel)
ON_BN_CLICKED(IDC_BUTTON_OUTPUTWORD, OnButtonOutputword)
ON_BN_CLICKED(IDC_RADIO_G1, OnRadioG1)
ON_BN_CLICKED(IDC_RADIO_G2, OnRadioG2)
ON_BN_CLICKED(IDC_RADIO_G3, OnRadioG3)
ON_BN_CLICKED(IDC_BUTTON_INPUTEXCEL2, OnButtonInputexcel2)
ON_BN_CLICKED(IDC_BUTTON_CACLU, OnButtonCaclu)
ON_EN_CHANGE(IDE_ZhuJu_X1, OnChangeZhuJuX1)
ON_EN_CHANGE(IDE_PaiJu_Y, OnChangePaiJuY)
ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
ON_EN_CHANGE(IDE_XiaTuoChengGaoDu, OnChangeXiaTuoChengGaoDu)
ON_BN_CLICKED(IDC_ADD_WAIJING, OnAddWaijing)
ON_BN_CLICKED(IDC_DEL_WAIJING, OnDelWaijing)
ON_BN_CLICKED(IDC_ADD_BIHOU, OnAddBihou)
ON_BN_CLICKED(IDC_DEL_BIHOU, OnDelBihou)
ON_BN_CLICKED(IDC_BTN_TESTZERO, OnBtnTestzero)
ON_EN_CHANGE(IDE_BuJu_Z, OnChangeBuJuZ)
	ON_BN_CLICKED(IDC_BTN_SECTION, OnBtnSection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSet message handlers

void CSet::OnSelchangeBuJuZ() 
{
	// TODO: Add your control notification handler code here
	CString str;
	//int i;
    Sort=m_Control_BuJu_Z.GetCurSel();
	if(Sort==0)                                        //0.6
	{
		BuJu_Z=0.6;
	}
	if(Sort==1)                                        //0.9
	{
		BuJu_Z=0.9;
	}
	if(Sort==2)                                        //1.2
	{
		BuJu_Z=1.2;
	}
}

void CSet::OnSelchangeZhuJuX() 
{
	// TODO: Add your control notification handler code here
	CString str;
	//int i;
    Sort=m_Control_ZhuJu_X.GetCurSel();
	if(Sort==0)                                        //0.6
	{
		ZhuJu_X=0.6;
	}
	if(Sort==1)                                        //0.9
	{
		ZhuJu_X=0.9;
	}
	if(Sort==2)                                        //1.2
	{
		ZhuJu_X=1.2;
	}
}

void CSet::OnSelchangeDanWeiFengHeZai()               //风荷载
{
	// TODO: Add your control notification handler code here
	CString str;
	//int i;
    Sort=m_Control_DanWei_FengHeZai.GetCurSel();
	if(Sort==0)                                        //kg
	{
		//	AfxMessageBox("1");
		FengHeZai=ini_FengHeZai;
		FengHeZai=9.8*FengHeZai/1000;
	}
	if(Sort==1)                                        //kN/(m*m)
	{
		//	AfxMessageBox("2");
		FengHeZai=ini_FengHeZai;
		FengHeZai=FengHeZai*ZhuJu_X*a*2.4;
	}
}

void CSet::OnSelchangeDanWeiFuJiaGouJian()           //附加构建
{
	// TODO: Add your control notification handler code here
	CString str;
	//int i;
    Sort=m_Control_DanWei_FuJiaGouJian.GetCurSel();
	
	if(Sort==0)                                        //kg
	{
		//	AfxMessageBox("1");
		FuJiaGouJian=ini_FuJiaGouJian;
		FuJiaGouJian=(9.8)*FuJiaGouJian/1000;
	}
	if(Sort==1)                                        //kN
	{
		//	AfxMessageBox("2");
		FuJiaGouJian=ini_FuJiaGouJian;
	}
	if(Sort==2)                                        //kN/(m*m)
	{
		//	AfxMessageBox("3");
        FuJiaGouJian=ini_FuJiaGouJian;
		FuJiaGouJian=FuJiaGouJian*a*ZhuJu_X*1.2;
	}
	if(Sort==3)                                        //t
	{
		//	AfxMessageBox("4");
		FuJiaGouJian=ini_FuJiaGouJian;
		FuJiaGouJian=FuJiaGouJian*9.8;
	}
}

void CSet::OnSelchangeDanWeiGangJinHunNingTu()       //钢筋
{
	// TODO: Add your control notification handler code here
	CString str;
	//int i;
    Sort=m_Control_DanWei_GangJinHunNingTu.GetCurSel();
	if(Sort==0)                                        //kg
	{
		//AfxMessageBox("1");
		GangJinHunNingTu=ini_GangJinHunNingTu;
        GangJinHunNingTu=9.8*GangJinHunNingTu/1000;
		str.Format("%f",GangJinHunNingTu);
		AfxMessageBox(str);
		
	}
	if(Sort==1)                                        //kN
	{
		//	AfxMessageBox("2");
		GangJinHunNingTu=ini_GangJinHunNingTu;
		GangJinHunNingTu=GangJinHunNingTu;
		str.Format("%f",GangJinHunNingTu);
		AfxMessageBox(str);
		
	}
	if(Sort==2)                                        //m3
	{
		//	AfxMessageBox("3");
		GangJinHunNingTu=ini_GangJinHunNingTu;
		GangJinHunNingTu=25*GangJinHunNingTu;
		str.Format("%f",GangJinHunNingTu);
		AfxMessageBox(str);
	}
	if(Sort==3)                                        //t
	{
		//	AfxMessageBox("4");
		GangJinHunNingTu=ini_GangJinHunNingTu;
		GangJinHunNingTu=9.8*GangJinHunNingTu;		
		str.Format("%f",GangJinHunNingTu);
		AfxMessageBox(str);
	}
}

void CSet::OnSelchangeDanWeiJiaoZhuZhenDao()         //振捣
{
	// TODO: Add your control notification handler code here
	CString str;
	//int i;
    Sort=m_Control_DanWei_JiaoZhuZhenDao.GetCurSel();
	if(Sort==0)                                        //kg
	{
		//	AfxMessageBox("1");
		JiaoZhuZhenDao=ini_JiaoZhuZhenDao;
		JiaoZhuZhenDao=9.8*JiaoZhuZhenDao/1000;
	}
	if(Sort==1)                                        //kN/(m*m)
	{
		//	AfxMessageBox("2");
		JiaoZhuZhenDao=ini_JiaoZhuZhenDao;
		for(int oo=1;oo<=10;oo++)
		{
			Length_Y+=(PaiJuDataSave[oo]*PaiJuCountSave[oo]);
		}
		JiaoZhuZhenDao=JiaoZhuZhenDao*a*ZhuJu_X*Length_Y;
	}
}

void CSet::OnSelchangeDanWeiMoBanFangLeng()     //模版方愣
{
	// TODO: Add your control notification handler code here
	CString str;
	//int i;
    Sort=m_Control_DanWei_MoBanFangLeng.GetCurSel();
	if(Sort==0)                                        //kg
	{
		//	AfxMessageBox("1");
		/*
		MoBanFangLeng=ini_MoBanFangLeng;
		MoBanFangLeng=9.8*MoBanFangLeng/1000;
		*/
		
	}
	if(Sort==1)                                        //kN
	{
		//	AfxMessageBox("2");
		MoBanFangLeng=ini_MoBanFangLeng;
	}
	if(Sort==2)                                        //kN/(m*m)
	{
		//	AfxMessageBox("3");
        for(int oo=1;oo<=10;oo++)
		{
			Length_Y+=(PaiJuDataSave[oo]*PaiJuCountSave[oo]);
		}
		MoBanFangLeng=MoBanFangLeng*a*ZhuJu_X*Length_Y;
		
	}
	if(Sort==3)                                        //t
	{
		//	AfxMessageBox("4");
		MoBanFangLeng=ini_MoBanFangLeng;
		MoBanFangLeng=9.8*MoBanFangLeng;
	}
}

void CSet::OnSelchangeDanWeiRenChaiJi()           //人材机
{
	// TODO: Add your control notification handler code here
	CString str;
	//int i;
    Sort=m_Control_DanWei_RenChaiJi.GetCurSel();
	if(Sort==0)                                        //kg
	{
	/*
	//	AfxMessageBox("1");
	RenChaiJi=ini_RenChaiJi;
	RenChaiJi=9.8*RenChaiJi/1000;
	str.Format("%f",RenChaiJi);
	AfxMessageBox(str);
		*/
	}
	if(Sort==1)                                        //kN
	{
		//	AfxMessageBox("2");
		RenChaiJi=ini_RenChaiJi;
		str.Format("%f",RenChaiJi);
		AfxMessageBox(str);
	}
	if(Sort==2)                                        //kN/(m*m)
	{
		
		//	AfxMessageBox("3");
		/*
		Deal_PaiJu_Y();
		RenChaiJi=ini_RenChaiJi;
		for(int oo=1;oo<=10;oo++)
		{
		Length_Y=Length_Y+(PaiJuDataSave[oo]*PaiJuCountSave[oo]);
		}
		RenChaiJi=RenChaiJi*a*ZhuJu_X*Length_Y;
		str.Format("%f",RenChaiJi);
		AfxMessageBox(str);
		*/
	}
	if(Sort==3)                                        //t
	{
		
		//	AfxMessageBox("4");
        /*
        RenChaiJi=ini_RenChaiJi;
		RenChaiJi=9.8*RenChaiJi;
		str.Format("%f",RenChaiJi);
		AfxMessageBox(str);
        */
	}
}

void CSet::OnChangeGangJinHunNingTu() 
{
	CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_GangJinHunNingTu,str);
	GangJinHunNingTu=atof(str);
	ini_GangJinHunNingTu=GangJinHunNingTu;
	
	
}

void CSet::OnChangeJiaoZhuZhenDao() 
{
	CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_JiaoZhuZhenDao,str);
	JiaoZhuZhenDao=atof(str);
	ini_JiaoZhuZhenDao=JiaoZhuZhenDao;
}

void CSet::OnChangeMoBanFangLeng() 
{
	CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_MoBanFangLeng,str);
	MoBanFangLeng=atof(str);	
	ini_MoBanFangLeng=MoBanFangLeng;
}

void CSet::OnChangeFuJiaGouJian() 
{
	CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_FuJiaGouJian,str);
	FuJiaGouJian=atof(str);
	ini_FuJiaGouJian=FuJiaGouJian;
}

void CSet::OnChangeRenChaiJi() 
{
    CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_RenChaiJi,str);
	RenChaiJi=atof(str);
	ini_RenChaiJi=RenChaiJi;
}

void CSet::OnChangeFengHeZai() 
{
	CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_FengHeZai,str);
	FengHeZai=atof(str);
	ini_FengHeZai=FengHeZai;
}

void CSet::OnChangeDiYiCiJiaoZhu() 
{
    CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_DiYiCiJiaoZhu,str);
	DiYiCiJiaoZhu=atof(str);
	UpdateData(TRUE);
	DiErCiJiaoZhu=1-DiYiCiJiaoZhu;
	
	m_DiErCiJiaoZhu.Format("%f",DiErCiJiaoZhu);
	UpdateData(FALSE);
	
	
}



void CSet::OnChangeDiYiCiYuYa() 
{
	CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_DiYiCiYuYa,str);
	DiYiCiYuYa=atof(str);
	
}

void CSet::OnChangeDiErCiYuYa() 
{
	CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_DiErCiYuYa,str);
	DiErCiYuYa=atof(str);
}

void CSet::OnChangeDiSanCiYuYa() 
{
	CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_DiSanCiYuYa,str);
	DiSanCiYuYa=atof(str);
}

void CSet::OnChangeDiErCiJiaoZhu() 
{
/*
CString str; 
UpdateData(TRUE);
GetDlgItemText(IDE_DiErCiJiaoZhu,str);
DiErCiJiaoZhu=atof(str);
	*/
}

void CSet::Deal_PaiJu_Y()
{
	CStdioFile File;	
   	CString str;
	string segment;	
	string paijusave;
	CString strtmp1;            //把用户输入的单词从txt文件中读出。
	CString strtmp2;
	CString sentence;
	CString sentence1;
    //double b;
	//float value1;
	//int   value2;
	int Length=0;
	int Empty=0;
	int q=0;
	int n=0;
	int j=0;					//容器中的单词计数
    int i=0;                    //检测位 
	int k=0;
	int l=0;
	int m=0;
	
	int iiii=0;
	int jjjj=0;
	
	
	
	
	//*********************************分跨方案******************************//
    UpdateData(TRUE);
	CEdit *pEdit=(CEdit *)GetDlgItem(IDE_PaiJu_Y);
	fstream File_Clear("Sentence.txt", fstream::out | ios_base::trunc); //每次存入前先清空前一次的内容
	File_Clear.close();
	UpdateData(TRUE);
	if(m_PaiJu_Y.IsEmpty())
	{
		AfxMessageBox("输入排距不能为空!",MB_ICONSTOP);	
		
	}
    else 
	{
		////////////////////////////////////////////////////
		CString Editstr = "";
		CString tempstr = "";
		pEdit->GetWindowText(tempstr);
		Editstr = tempstr+" ";
		char Strbuf[30];
		char Countbuf[10];
		char Databuf[20];
		for(i = 0,j=0; i <Editstr.GetLength();i++)
		{
			Strbuf[j++] = Editstr.GetAt(i);
			if((Editstr.GetAt(i)==',')||(Editstr.GetAt(i)==' '))
			{
				Strbuf[j]='\0';
				int AtFlag = 0;
				for(k=0;k<j;k++)
				{
					if(Strbuf[k]=='@')
					{
						AtFlag=1;
						for(l=0;l<k;l++)
						{
							Countbuf[l]=Strbuf[l];
						}
						Countbuf[l]='\0';
						PaiJuCountSave[iiii++] = atoi(Countbuf);
						for(l=k;l<j;l++)
						{
							Databuf[l-k]=Strbuf[l+1];
						}
						Databuf[l-k]='\0';
						PaiJuDataSave[jjjj++]=atof(Databuf);
						PaiJuCount++;
						break;
					}
				}
				if(AtFlag==0)
				{
					tempstr.Format("%s",Strbuf);
					PaiJuCountSave[iiii++] = 1;
					PaiJuDataSave[jjjj++]=atof(tempstr);
					PaiJuCount++;
					//float tempflo = atof(tempstr);
				}
				j=0;
			}
		}
	}
	Count_Y=iiii;
	PaiJuGeShu_Y=0;
	for(int iii=0;iii<PaiJuCount;iii++)
	{
		PaiJuGeShu_Y=PaiJuGeShu_Y+PaiJuCountSave[iii];
	}
	PaiJuGeShu_Y=PaiJuGeShu_Y+1;
	return ;
	/*		Count_Y=iiii;
	PaiJuGeShu_Y = 0;
	for(int i=0;i<iiii;i++)
	{
	PaiJuGeShu_Y=PaiJuGeShu_Y+PaiJuCountSave[i];
	}
	return;
	//////////////////////////////////////////////////////
	Empty=0;
	sentence=m_PaiJu_Y;
	Length=sentence.GetLength();
	str.Format("%d",Length);
	//	AfxMessageBox(str);
	
	  paijusave= LPCSTR(sentence); 
	  for(i=1;i<=Length;i++)
	  {
	  if(paijusave[i]=='@')
	  {
	  paijusave[i]=' ';
	  }
	  }
	  
		str.Format("%s",paijusave.c_str());
		sentence=str;
		//AfxMessageBox(str);
		
		  
			
			  File.Open("Sentence.txt",CFile::modeReadWrite); 
			  File.WriteString(sentence);                 //把"Sentence.txt"里的内容赋值给string变量sentence
			  File.Close();
			  ifstream fileout("Sentence.txt");
			  i=0;
			  while(fileout>>segment)                    //把用户输入的数据输入Re[n]
			  {
			  CString str;
			  if(i%2==0)
			  {
			  value2=atoi(segment.c_str());
			  PaiJuCountSave[iiii]=value2;
			  iiii++;
			  }
			  if(i%2==1)
			  {
			  value1 = atof(segment.c_str());
			  PaiJuDataSave[jjjj]=value1;
			  jjjj++;
			  }
			  i++;
			  PaiJuCount++;
			  }
			  
				
				  
					}
					
					  for(int iii=0;iii<(PaiJuCount/2);iii++)
					  {
					  PaiJuGeShu_Y=PaiJuGeShu_Y+PaiJuCountSave[iii];
					  }
    PaiJuGeShu_Y=PaiJuGeShu_Y+1;*/
}



void CSet::Deal_ZhuJu_X()
{
	int i=0,j=0,k=0,l=0,m=0,n=0;
	//*********************************分跨方案******************************//
    UpdateData(TRUE);
	CEdit *pEdit=(CEdit *)GetDlgItem(IDE_ZhuJu_X1);
	fstream File_Clear("Sentence.txt", fstream::out | ios_base::trunc); //每次存入前先清空前一次的内容
	File_Clear.close();
	UpdateData(TRUE);
	CString Editstr = "";
	CString tempstr = "";
	pEdit->GetWindowText(tempstr);
	if(tempstr.IsEmpty())
	{
		AfxMessageBox("输入柱距不能为空!",MB_ICONSTOP);	
		
	}
    else 
	{
		////////////////////////////////////////////////////
		Editstr = tempstr+" ";
		char Strbuf[30];
		char Countbuf[10];
		char Databuf[20];
		for(i = 0,j=0; i <Editstr.GetLength();i++)
		{
			Strbuf[j++] = Editstr.GetAt(i);
			if((Editstr.GetAt(i)==',')||(Editstr.GetAt(i)==' '))
			{
				Strbuf[j]='\0';
				int AtFlag = 0;
				for(k=0;k<j;k++)
				{
					if(Strbuf[k]=='@')
					{
						AtFlag=1;
						for(l=0;l<k;l++)
						{
							Countbuf[l]=Strbuf[l];
						}
						Countbuf[l]='\0';
						ZhuJuCountSave[m++] = atoi(Countbuf);
						for(l=k;l<j;l++)
						{
							Databuf[l-k]=Strbuf[l+1];
						}
						Databuf[l-k]='\0';
						ZhuJuDataSave[n++]=atof(Databuf);
						break;
					}
				}
				if(AtFlag==0)
				{
					tempstr.Format("%s",Strbuf);
					ZhuJuCountSave[m++] = 1;
					ZhuJuDataSave[n++]=atof(tempstr);
					//float tempflo = atof(tempstr);
				}
				j=0;
			}
		}
		Count_X=m;
		ZhuJuGeShu_X = 0;
		for(i=0;i<m;i++)
		{
			ZhuJuGeShu_X=ZhuJuGeShu_X+ZhuJuCountSave[i];
		}
		ZhuJuGeShu_X=ZhuJuGeShu_X+1;
	}
	return;
}

void CSet::Deal_BuJu_Z()
{
	int i=0,j=0,k=0,l=0,m=0,n=0;
	//*********************************分跨方案******************************//
    UpdateData(TRUE);
	CEdit *pEdit=(CEdit *)GetDlgItem(IDE_BuJu_Z);
	fstream File_Clear("Sentence.txt", fstream::out | ios_base::trunc); //每次存入前先清空前一次的内容
	File_Clear.close();
	UpdateData(TRUE);
	CString Editstr = "";
	CString tempstr = "";
	pEdit->GetWindowText(tempstr);
	if(tempstr.IsEmpty())
	{
		AfxMessageBox("输入步距不能为空!",MB_ICONSTOP);	
		
	}
    else 
	{
		////////////////////////////////////////////////////
		BuJuCountSave[m++] = 1;
		BuJuDataSave[n++]=XiaBuTuoChengGaoDu;
		//BuJuCountSave[m++] = 1;
		//BuJuDataSave[n++]=SaoDiGanGaoDu-XiaBuTuoChengGaoDu;
		BuJuCountSave[m++] = 1;
		BuJuDataSave[n++]=0.3;
		Editstr = tempstr+" ";
		char Strbuf[30];
		char Countbuf[10];
		char Databuf[20];
		for(i = 0,j=0; i <Editstr.GetLength();i++)
		{
			Strbuf[j++] = Editstr.GetAt(i);
			if((Editstr.GetAt(i)==',')||(Editstr.GetAt(i)==' '))
			{
				Strbuf[j]='\0';
				int AtFlag = 0;
				for(k=0;k<j;k++)
				{
					if(Strbuf[k]=='@')
					{
						AtFlag=1;
						for(l=0;l<k;l++)
						{
							Countbuf[l]=Strbuf[l];
						}
						Countbuf[l]='\0';
						BuJuCountSave[m++] = atoi(Countbuf);
						for(l=k;l<j;l++)
						{
							Databuf[l-k]=Strbuf[l+1];
						}
						Databuf[l-k]='\0';
						BuJuDataSave[n++]=atof(Databuf);
						break;
					}
				}
				if(AtFlag==0)
				{
					tempstr.Format("%s",Strbuf);
					BuJuCountSave[m++] = 1;
					BuJuDataSave[n++]=atof(tempstr);
					//float tempflo = atof(tempstr);
				}
				j=0;
			}
		}
		BuJuCountSave[m++] = 1;
		BuJuDataSave[n++]=DingCengXuanBiChangDu;
		Count_Z=m;
		BuJuGeShu_Z = 0;
		for(i=0;i<m;i++)
		{
			BuJuGeShu_Z=BuJuGeShu_Z+BuJuCountSave[i];
		}
		BuJuGeShu_Z=BuJuGeShu_Z+1;
	}
	return;	
}
void CSet::Ontest() 
{
	// TODO: Add your control notification handler code here
	CString str;
	//	init();
	//	Deal_PaiJu_Y();
	
	str.Format("a:%d,b:%d,c:%d,dc:%d",a,b,c,dc);
	AfxMessageBox(str);
}



void CSet::OnChangeZhuJuX() 
{
	CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_ZhuJuX,str);
	ZhuJuGeShu_X=atoi(str);
	ZhuJuGeShu_X=ZhuJuGeShu_X+1;
	Length_X=ZhuJuGeShu_X*ZhuJu_X;
}

void CSet::OnChangeBuJuZ() 
{
	return;
   	CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_BuJuZ,str);
	BuJuGeShu_Z=atoi(str);
	Length_Z=BuJuGeShu_Z*BuJu_Z;
	
}

void CSet::OnChangeSaoDiGanGaoDu() 
{
	CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_SaoDiGanGaoDu,str);
	SaoDiGanGaoDu=atof(str);
	if(SaoDiGanGaoDu>0.35)
	{
		MessageBox("扫地杆距离地面高度应小于等于0.35m","提示:", MB_ICONEXCLAMATION);
	}
	
}

void CSet::OnChangeDingCengXuanBi() 
{
	CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_DingCengXuanBi,str);
	DingCengXuanBiChangDu=atof(str);
	if(DingCengXuanBiChangDu>0.7)
	{
		MessageBox("扫地杆距离地面高度应小于等于0.35m","提示:", MB_ICONEXCLAMATION);
	}
}

void CSet::OnOutput() 
{
	// TODO: Add your control notification handler code here
	if(FALSE==AddDeviation())
		return;
	int N=3;
	int num=1;//编号
	int i=0;//计数器
	int j=0;
	int k=0;
	int l=0;
	int m=0;
	int n=0;
	CString str;
	init();
	GetDlgItemText(IDE_SaoDiGanGaoDu,str);
	SaoDiGanGaoDu=atof(str);
	GetDlgItemText(IDE_DingCengXuanBi,str);
	DingCengXuanBiChangDu=atof(str);
	GetDlgItemText(IDE_XiaTuoChengGaoDu,str);
	XiaBuTuoChengGaoDu=atof(str);
    Deal_PaiJu_Y();
    Deal_ZhuJu_X();
    Deal_BuJu_Z();
	
	
	if(BuJuGeShu_Z==0)
	{
		AfxMessageBox("步距输入无效，当前个数为0");
		return ;
	}
	//BuJuGeShu_Z=BuJuGeShu_Z+4;
	
	NodeZongShu=BuJuGeShu_Z*ZhuJuGeShu_X*PaiJuGeShu_Y;
    //NodeZong=new Node[NodeZongShu+1];
	vector < Node > NodeZong;
	NodeZong.resize(NodeZongShu*3);
	for(i=0;i<NodeZongShu*3;i++)
	{
		NodeZong[i].Num=0;
		NodeZong[i].x=0;
		NodeZong[i].y=0;
		NodeZong[i].z=0;
	}
	
	
	
	
	////////////////////////////////////////////////////////////////////////////////
	int BB,BB1,ZZ,ZZ1,PP,PP1;
    //Deal_PaiJu_Y();
    //Deal_ZhuJu_X();
    //Deal_BuJu_Z();
	int tempBu=0;
	int tempZhu=0;
	int tempPai=0;
	//CString filename="h:\\midas.txt";
	//filename=filename+".mct";
	//CStdioFile File;
	//File.Open(filename,CFile::modeCreate|CFile::modeReadWrite);//如果文件事先不存在的话，就需要CFile::modeCreate，否则就不需要。
	//CString TxtStr="";
	float CurZVal = 0.0;
	float CurYVal = 0.0;
	float CurXVal = 0.0;
	for(BB=-1;BB<Count_Z;BB++)
	{
		if(BB==-1)
			tempBu=1;
		else
			tempBu=BuJuCountSave[BB];
		for(BB1=0;BB1<tempBu;BB1++)
		{
			if(BB==-1)
				CurZVal=0.0;
			else
				CurZVal=CurZVal+BuJuDataSave[BB];
			CurXVal = 0.0;
			for(ZZ=-1;ZZ<Count_X;ZZ++)
			{
				if(ZZ==-1)
					tempZhu=1;
				else
					tempZhu=ZhuJuCountSave[ZZ];
				for(ZZ1=0;ZZ1<tempZhu;ZZ1++)
				{
					if(ZZ==-1)
						CurXVal=0.0;
					else
						CurXVal=CurXVal+ZhuJuDataSave[ZZ];
					CurYVal=0.0;
					for(PP=-1;PP<Count_Y;PP++)
					{
						if(PP==-1)
							tempPai=1;
						else
							tempPai=PaiJuCountSave[PP];
						for(PP1=0;PP1<tempPai;PP1++)
						{
							if(PP==-1)
								CurYVal=0.0;
							else
								CurYVal=CurYVal+PaiJuDataSave[PP];
							NodeZong[num].Num=num;
							NodeZong[num].x=CurXVal;
							NodeZong[num].y=CurYVal;
							NodeZong[num++].z=CurZVal;
							//TxtStr.Format("%.2f,%.2f,%.2f\n",CurXVal,CurYVal,CurZVal);
							//File.WriteString(TxtStr);
						}
					}
				}
			}
		}
	}
	////////////////////////扫地杆节点生成//////////////
	float maxX=CurXVal;
	float maxY=CurYVal;
	float maxZ=CurZVal;
	CurZVal=SaoDiGanGaoDu;
	CurXVal = 0.0;
	int saodiStart=num;
	int saodiEnd;
	for(ZZ=-1;ZZ<Count_X;ZZ++)
	{
		if(ZZ==-1)
			tempZhu=1;
		else
			tempZhu=ZhuJuCountSave[ZZ];
		for(ZZ1=0;ZZ1<tempZhu;ZZ1++)
		{
			if(ZZ==-1)
				CurXVal=0.0;
			else
				CurXVal=CurXVal+ZhuJuDataSave[ZZ];
			CurYVal=0.0;
			for(PP=-1;PP<Count_Y;PP++)
			{
				if(PP==-1)
					tempPai=1;
				else
					tempPai=PaiJuCountSave[PP];
				for(PP1=0;PP1<tempPai;PP1++)
				{
					if(PP==-1)
						CurYVal=0.0;
					else
						CurYVal=CurYVal+PaiJuDataSave[PP];
					if((CurXVal==0.0)||(CurXVal==maxX)||(CurYVal==0.0)||(CurYVal==maxY))
					{
						NodeZong[num].Num=num;
						NodeZong[num].x=CurXVal;
						NodeZong[num].y=CurYVal;
						NodeZong[num++].z=CurZVal;
					}
				}
			}
		}
	}
	saodiEnd=num-1;
	//File.Close();
	fline XZline[10][30];
	memset(XZline,0,sizeof(XZline)/sizeof(XZline[0][0])*sizeof(fline));
	float tempZ=0.0;
	for(i=0;i<Count_Z;i++)
	{
		for(j=0;j<BuJuCountSave[i];j++)
		{
			tempZ=tempZ+BuJuDataSave[i];
		}
	}
	float tempY=0.0;
	for(i=0;i<Count_Y;i++)
	{
		for(j=0;j<PaiJuCountSave[i];j++)
		{
			tempY=tempY+PaiJuDataSave[i];
		}
	}
	float tempX=0.0;
	for(i=0;i<Count_X;i++)
	{
		for(j=0;j<ZhuJuCountSave[i];j++)
		{
			tempX=tempX+ZhuJuDataSave[i];
		}
	}
	int countline=0;
	for(i=0;i<1;i++)
	{
		XZline[i][countline].xs=0.0;
		XZline[i][countline].zs=0.3+XiaBuTuoChengGaoDu;
		if(tempX+XZline[i][countline].zs>tempZ)
		{
			XZline[i][countline].xe=tempZ-XZline[i][countline].zs;
			XZline[i][countline++].ze=tempZ;
		}
		else
		{
			XZline[i][countline].ze=XZline[i][countline].zs+tempX;
			XZline[i][countline++].xe=tempX;
		}
		//int abc=(tempZ-0.3-XiaBuTuoChengGaoDu)/4.5;
		for(j=0;j<(tempZ-0.3-XiaBuTuoChengGaoDu)/4.5-1;j++)
		{//xz面正向中斜线及其以上斜线
			XZline[i][countline].xs=0.0;
			XZline[i][countline].zs=0.3+XiaBuTuoChengGaoDu+j*4.5+4.5;
			if(tempX+XZline[i][countline].zs>tempZ)
			{
				XZline[i][countline].xe=tempZ-XZline[i][countline].zs;
				XZline[i][countline++].ze=tempZ;
			}
			else
			{
				XZline[i][countline].ze=XZline[i][countline].zs+tempX;
				XZline[i][countline++].xe=tempX;
			}
		}
		int GettempStart=countline;
		for(j=1;j<tempX/4.5;j++)
		{//xz面正向中斜线以下斜线
			XZline[i][countline].zs=0.0;
			XZline[i][countline].xs=j*4.5;
			if(tempX-XZline[i][countline].xs>tempZ)
			{
				XZline[i][countline].xe=tempZ+XZline[i][countline].xs;
				XZline[i][countline++].ze=tempZ;
			}
			else
			{
				XZline[i][countline].ze=tempX-XZline[i][countline].xs+0.3+XiaBuTuoChengGaoDu;
				XZline[i][countline++].xe=tempX;
			}
		}
		int GettempEnd=countline;
		for(j=GettempStart;j<GettempEnd;j++)
		{//xz面反向中斜线以下斜线
			XZline[i][countline].xs=XZline[i][j].xs;
			XZline[i][countline].zs=XZline[i][j].zs;
			if(XZline[i][countline].xs+XZline[i][countline].zs<tempZ)
			{
				XZline[i][countline].ze=XZline[i][countline].xs+XZline[i][countline].zs;
				XZline[i][countline++].xe=0.0;
			}
			else
			{
				XZline[i][countline].xe=XZline[i][countline].xs-tempY+0.3+XiaBuTuoChengGaoDu;
				XZline[i][countline++].ze=tempZ;
			}
		}
		float tempfloat=XZline[i][countline-1].xs;
		if(tempfloat==0.0)
		{
			XZline[i][countline].zs=0.3+XiaBuTuoChengGaoDu;
			XZline[i][countline].xs=tempX;
		}
		else
		{
			XZline[i][countline].zs=0.3+XiaBuTuoChengGaoDu+(4.5-(tempX-tempfloat));
			XZline[i][countline].xs=tempX;
			if(tempZ-XZline[i][countline].zs<tempX)
			{
				XZline[i][countline].xe=tempX-(tempZ-XZline[i][countline].zs);
				XZline[i][countline++].ze=tempZ;
			}
			else
			{
				XZline[i][countline].ze=XZline[i][countline].zs+tempX;
				XZline[i][countline++].xe=0.0;
			}
		}
		for(j=0;j<(tempZ-0.3-XiaBuTuoChengGaoDu-(4.5-(tempX-tempfloat)))/4.5-1;j++)
		{//xz面反向中斜线以上斜线
			XZline[i][countline].xs=tempX;
			XZline[i][countline].zs=XZline[i][countline-1].zs+4.5;
			if(tempZ-XZline[i][countline].zs<tempX)
			{
				XZline[i][countline].xe=tempX-(tempZ-XZline[i][countline].zs);
				XZline[i][countline++].ze=tempZ;
			}
			else
			{
				XZline[i][countline].ze=XZline[i][countline].zs+tempX;
				XZline[i][countline++].xe=0.0;
			}
		}
	}
	int jiandaoStart=num;
	Node XZNode[400];//从1开始填充，存储剪刀撑新生节点
	memset(XZNode,0,sizeof(XZNode)/sizeof(XZNode[0])*sizeof(Node));
	for(i=0;i<countline;i++)
	{
	tempX=0.0;
		for(j=0;j<Count_X;j++)
		{
			for(k=0;k<ZhuJuCountSave[j];k++)
			{
				if(((XZline[0][i].xs<=tempX)&&(tempX<=XZline[0][i].xe))||((XZline[0][i].xs>=tempX)&&(tempX>=XZline[0][i].xe)))
				{
					//XZNode[num-jiandaoStart+1].Num=num;
					//XZNode[num-jiandaoStart+1].x=tempX;
					//XZNode[num-jiandaoStart+1].y=0.0;
					//XZNode[num-jiandaoStart+1].z=(tempX-XZline[0][i].xs)/(XZline[0][i].xe-XZline[0][i].xs)*(XZline[0][i].ze-XZline[0][i].zs)+XZline[0][i].zs;
					NodeZong[num].Num=num;
					NodeZong[num].x=tempX;
					NodeZong[num].y=0.0;
					NodeZong[num++].z=(tempX-XZline[0][i].xs)/(XZline[0][i].xe-XZline[0][i].xs)*(XZline[0][i].ze-XZline[0][i].zs)+XZline[0][i].zs;
				}
				tempX=tempX+ZhuJuDataSave[j];
				if(tempX==maxX)
				{
					if(((XZline[0][i].xs<=tempX)&&(tempX<=XZline[0][i].xe))||((XZline[0][i].xs>=tempX)&&(tempX>=XZline[0][i].xe)))
					{
						//XZNode[num-jiandaoStart+1].Num=num;
						//XZNode[num-jiandaoStart+1].x=tempX;
						//XZNode[num-jiandaoStart+1].y=0.0;
						//XZNode[num-jiandaoStart+1].z=(tempX-XZline[0][i].xs)/(XZline[0][i].xe-XZline[0][i].xs)*(XZline[0][i].ze-XZline[0][i].zs)+XZline[0][i].zs;
						NodeZong[num].Num=num;
						NodeZong[num].x=tempX;
						NodeZong[num].y=0.0;
						NodeZong[num++].z=(tempX-XZline[0][i].xs)/(XZline[0][i].xe-XZline[0][i].xs)*(XZline[0][i].ze-XZline[0][i].zs)+XZline[0][i].zs;
						if(XZline[0][i].xe-XZline[0][i].xs==0)
							num--;
					}
				}
			}
		}
	}
	int jiandaoEnd=num-1;
	
	//**********************单元************************/
	int MeiCengDanYuanShu=0;
	int ZongDanYuanShu=0;
	
	
	
	//***********************文件输出****************//
	
	CStdioFile  File1;
	CString filename;
	CString filename1;
	CString fileext;//文件扩展名
	CString filepathname;
	CFileDialog fpdlg(FALSE,  NULL,  NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"(文件类型)(*.mct)|*.mct|(文件类型)(*.txt)|*.txt|所有文件(*.*)|*.*||",NULL);
	
	if(fpdlg.DoModal() == IDOK)
	{
		filename = fpdlg.GetFileName();
		fileext = fpdlg.GetFileExt();//文件扩展名
		filepathname = fpdlg.GetPathName(); 
	}
	else if (fpdlg.DoModal() == IDCANCEL)
	{
		AfxMessageBox("没有读取sgy文件");
		return;
	}
	
	filename1=filename;
	File1.Open(filename1,CFile::modeCreate|CFile::modeReadWrite);//如果文件事先不存在的话，就需要CFile::modeCreate，否则就不需要。
	// str=_T("     计算结果如下:         ");
	// File1.WriteString(str);
	// File1.WriteString("\n");	   
	str.Format(";---------------------------------------------------------------------------\n");
	File1.WriteString(str);
	str.Format(";  MIDAS/Civil Text(MCT) File.\n");
	File1.WriteString(str);
	CTime t = CTime::GetCurrentTime();
	str.Format(";  Date : %d/%d/%d\n",t.GetYear(),t.GetMonth(),t.GetDay());
	File1.WriteString(str);
	str.Format(";---------------------------------------------------------------------------\n\n");
	File1.WriteString(str);
	str.Format("*VERSION\n   8.0.5\n\n");
	File1.WriteString(str);
	str.Format("*UNIT    ; Unit System\n; FORCE, LENGTH, HEAT, TEMPER\n   KN   , M, KJ, C\n\n");
	File1.WriteString(str);
	
	
	
	
	//***************************节点输出前缀*************************//
	str=_T("*NODE    ; Nodes");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	str=_T("; iNO, X, Y, Z");
	File1.WriteString(str);
	File1.WriteString("\n");
	for(int h=1;h<num;h++)
	{
		str.Format("%d,  %7.2f , %7.2f , %7.2f",NodeZong[h].Num,NodeZong[h].x,NodeZong[h].y,NodeZong[h].z);
		File1.WriteString(str);
		File1.WriteString("\n");
	}
	File1.WriteString("\n");
	File1.WriteString("\n");
	str.Format("剪刀撑节点,%dto%d,0\n",jiandaoStart,jiandaoEnd);
	str="*GROUP    ; Group\n; NAME, NODE_LIST, ELEM_LIST, PLANE_TYPE\n"+str;
	File1.WriteString(str);

	
	//***************************非斜向单元输出************************//
	str=_T("*ELEMENT ");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");

	
	//File1.Close();
	//AfxMessageBox("success");
	//return;
	
	/*
	Dim dy, dc, qd, zd As Integer
	' dy单元编号 dc单层节点数，qd单元起点节点号，zd单元重点节点号
	dy = 0
	dc = (a + 1) * (b + 1)
	*/
	
	a=ZhuJuGeShu_X-1;
	b=PaiJuGeShu_Y-1;
	c=BuJuGeShu_Z-3;//-3;田海涛20:00 2016/9/8
	float z=BuJu_Z;
	int HorizPoleNum=0;//记录横向杆总个数
	dy=0;
	dc=(a+1)*(b+1);
	int vc=0;
	int va=0;
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////首次执行整个单元建模过程以计算总单元数////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	for(vc=2;vc<=c+1;vc++)
	{
		for(va=1;va<=a+1;va++)
		{
			for(qd=(va-1)*b+va+vc*dc;qd<=va-1+va*b+vc*dc;qd++)
			{
				dy=dy+1;
			}
		}
		
	}
	//横杆2/2
	for(vc=2;vc<=c+1;vc++)
	{ 
		for(qd=1+dc*vc;qd<=a*(b+1)+dc*vc;qd++)
		{
			dy=dy+1;
		}
		
	}
	HorizPoleNum = dy;
	//立杆
	for(vc=0;vc<=c+1;vc++)
	{ 
		for(qd=1+dc*vc;qd<=(a+1)*(b+1)+dc*vc;qd++)
		{
			dy=dy+1;
		}
		
	}
	int ZCount=0;
	int ZNum[20];
	float tempZDate=0.0;
	int tempZNum=2;
	ZNum[ZCount++]=1+ZhuJuGeShu_X*PaiJuGeShu_Y*tempZNum;
	for(i=2;i<Count_Z-1;i++)
	{
		for(j=0;j<BuJuCountSave[i];j++)
		{
			tempZDate=tempZDate+BuJuDataSave[i];
			if(tempZDate>=4.8)
			{
				ZNum[ZCount++]=1+ZhuJuGeShu_X*PaiJuGeShu_Y*tempZNum;
				tempZDate=BuJuDataSave[i];
			}
			tempZNum++;
		}
	}
	if(ZNum[ZCount-1]!=1+ZhuJuGeShu_X*PaiJuGeShu_Y*(BuJuGeShu_Z-1))
	{
		ZNum[ZCount++]=1+ZhuJuGeShu_X*PaiJuGeShu_Y*(BuJuGeShu_Z-2);
	}
	int Turn = 1;
	//////////////////////水平剪刀撑///////////////////////////
	for(i=0;i<ZCount;i++)
	{
		//int Max=ZhuJuGeShu_X>PaiJuGeShu_Y?ZhuJuGeShu_X:PaiJuGeShu_Y;
		//int Min=ZhuJuGeShu_X<PaiJuGeShu_Y?ZhuJuGeShu_X:PaiJuGeShu_Y;
		for(k=0;k<=(PaiJuGeShu_Y-1-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=ZNum[i]+k*5;
			for(j=1+5*k;j<=PaiJuGeShu_Y-1;j++)
			{
				dy = dy + 1;
				if((j-5*k)==ZhuJuGeShu_X-1)
					break;
			}
		}
		for(k=1;k<=(ZhuJuGeShu_X-1-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=ZNum[i]+k*5*PaiJuGeShu_Y;
			for(j=1+5*k;j<=ZhuJuGeShu_X-1;j++)
			{
				dy = dy + 1;
				if((j-5*k)==PaiJuGeShu_Y-1)
					break;
			}
		}
		for(k=1;k<=(ZhuJuGeShu_X-1-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=ZNum[i]+k*5*PaiJuGeShu_Y;
			for(j=1;j<=5*k;j++)
			{
				dy = dy + 1;
				if(j==PaiJuGeShu_Y-1)
					break;
			}
		}
		if((ZhuJuGeShu_X-1)%5>=3)
		{
			zd=ZNum[i]+ZhuJuGeShu_X*PaiJuGeShu_Y-PaiJuGeShu_Y;
			for(j=1;j<=PaiJuGeShu_Y-1;j++)
			{
				dy = dy + 1;
				if(j==ZhuJuGeShu_X-1)
					break;
			}
		}
		for(k=(ZhuJuGeShu_X-1)%5;k<PaiJuGeShu_Y-1;k+=5)//1代表允许末尾多出段数
		{
			zd=ZNum[i]+ZhuJuGeShu_X*PaiJuGeShu_Y-PaiJuGeShu_Y+k;
			for(j=1;j<=ZhuJuGeShu_X-1;j++)
			{
				dy = dy + 1;
				if(j+k==PaiJuGeShu_Y-1)
					break;
			}
		}
	}
	//File1.Close();
	//AfxMessageBox("success");
	//return;
	//File1.WriteString(";;横桥向剪刀撑\n");
	
	//////////////////////////////////////////////////////////
	///////////////////////横桥向剪刀撑//////////////////////////////
	for(i=0;i<=(ZhuJuGeShu_X-1-1)/5+1;i++)
	{
		//int Max=ZhuJuGeShu_X>PaiJuGeShu_Y?ZhuJuGeShu_X:PaiJuGeShu_Y;
		//int Min=ZhuJuGeShu_X<PaiJuGeShu_Y?ZhuJuGeShu_X:PaiJuGeShu_Y;
		for(k=0;k<=(PaiJuGeShu_Y-1-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=1+2*ZhuJuGeShu_X*PaiJuGeShu_Y+i*5*PaiJuGeShu_Y+k*5;
			if(i==(ZhuJuGeShu_X-1-1)/5+1)
				zd=1+2*ZhuJuGeShu_X*PaiJuGeShu_Y+(ZhuJuGeShu_X-1)*PaiJuGeShu_Y+k*5;
			for(j=1;j<=BuJuGeShu_Z-5;j++)
			{
				dy = dy + 1;
				if((j+5*k)==PaiJuGeShu_Y-1)
					break;
			}
		}
		for(k=1;k<=(BuJuGeShu_Z-5)/5;k++)//1代表允许末尾多出段数
		{
			zd=1+3*ZhuJuGeShu_X*PaiJuGeShu_Y+i*5*PaiJuGeShu_Y+k*ZhuJuGeShu_X*PaiJuGeShu_Y*5;
			if(i==(ZhuJuGeShu_X-1-1)/5+1)
				zd=1+3*ZhuJuGeShu_X*PaiJuGeShu_Y+(ZhuJuGeShu_X-1)*PaiJuGeShu_Y+k*ZhuJuGeShu_X*PaiJuGeShu_Y*5;
			for(j=1;j<=PaiJuGeShu_Y-1;j++)
			{
				dy = dy + 1;
				if((j+5*k)==BuJuGeShu_Z-5)
					break;
			}
		}
		/////////////////////////////0915暂停
		for(k=1;k<=(PaiJuGeShu_Y-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=1+2*ZhuJuGeShu_X*PaiJuGeShu_Y+i*5*PaiJuGeShu_Y+k*5;
			if(i==(ZhuJuGeShu_X-1-1)/5+1)
				zd=1+2*ZhuJuGeShu_X*PaiJuGeShu_Y+(ZhuJuGeShu_X-1)*PaiJuGeShu_Y+k*5;
			for(j=1;j<=5*k;j++)
			{
				dy = dy + 1;
				if(j==BuJuGeShu_Z-5)
					break;
			}
		}
		if((PaiJuGeShu_Y-1)%5>=3)
		{
			zd=2*ZhuJuGeShu_X*PaiJuGeShu_Y+i*5*PaiJuGeShu_Y+PaiJuGeShu_Y;
			if(i==(ZhuJuGeShu_X-1-1)/5+1)
				zd=3*ZhuJuGeShu_X*PaiJuGeShu_Y;
			for(j=1;j<=PaiJuGeShu_Y-1;j++)
			{
				dy = dy + 1;
				if(j==BuJuGeShu_Z-5)
					break;
			}
		}
		for(k=(PaiJuGeShu_Y-1)%5+3;k<BuJuGeShu_Z-3;k+=5)//1代表允许末尾多出段数
		{
			zd=PaiJuGeShu_Y+k*ZhuJuGeShu_X*PaiJuGeShu_Y+i*5*PaiJuGeShu_Y;
			if(i==(ZhuJuGeShu_X-1-1)/5+1)
				zd=PaiJuGeShu_Y+k*ZhuJuGeShu_X*PaiJuGeShu_Y+(ZhuJuGeShu_X-1)*PaiJuGeShu_Y;
			for(j=1;j+k<=BuJuGeShu_Z-2;j++)
			{
				dy = dy + 1;
				if(j==PaiJuGeShu_Y-1)
					break;
			}
		}
	}
	
	///////////////////////顺桥向剪刀撑//////////////////////////////
	for(i=0;i<=(PaiJuGeShu_Y-1-1)/5+1;i++)
	{
		//int Max=PaiJuGeShu_Y>ZhuJuGeShu_X?PaiJuGeShu_Y:ZhuJuGeShu_X;
		//int Min=PaiJuGeShu_Y<ZhuJuGeShu_X?PaiJuGeShu_Y:ZhuJuGeShu_X;
		for(k=0;k<=(ZhuJuGeShu_X-1-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=1+2*PaiJuGeShu_Y*ZhuJuGeShu_X+i*5+k*5*PaiJuGeShu_Y;
			if(i==(PaiJuGeShu_Y-1-1)/5+1)
				zd=1+2*PaiJuGeShu_Y*ZhuJuGeShu_X+PaiJuGeShu_Y-1+k*5*PaiJuGeShu_Y;
			for(j=1;j<=BuJuGeShu_Z-5;j++)
			{
				dy = dy + 1;
				if((j+5*k)==ZhuJuGeShu_X-1)
					break;
			}
		}
		for(k=1;k<=(BuJuGeShu_Z-5)/5;k++)//1代表允许末尾多出段数
		{
			zd=1+3*PaiJuGeShu_Y*ZhuJuGeShu_X+i*5+k*5*PaiJuGeShu_Y*ZhuJuGeShu_X;
			if(i==(PaiJuGeShu_Y-1-1)/5+1)
				zd=3*PaiJuGeShu_Y*ZhuJuGeShu_X+PaiJuGeShu_Y+k*ZhuJuGeShu_X*PaiJuGeShu_Y*5;
			for(j=1;j<=ZhuJuGeShu_X-1;j++)
			{
				dy = dy + 1;
				if((j+5*k)==BuJuGeShu_Z-5)
					break;
			}
		}
		/////////////////////////////0915暂停
		for(k=1;k<=(ZhuJuGeShu_X-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=1+2*PaiJuGeShu_Y*ZhuJuGeShu_X+i*5+k*5*PaiJuGeShu_Y;
			if(i==(PaiJuGeShu_Y-1-1)/5+1)
				zd=2*PaiJuGeShu_Y*ZhuJuGeShu_X+PaiJuGeShu_Y+k*5*PaiJuGeShu_Y;
			for(j=1;j<=5*k;j++)
			{
				dy = dy + 1;
				if(j==BuJuGeShu_Z-5)
					break;
			}
		}
		if((ZhuJuGeShu_X-1)%5>=3)
		{
			zd=1+2*PaiJuGeShu_Y*ZhuJuGeShu_X+i*5+(ZhuJuGeShu_X-1)*PaiJuGeShu_Y;
			if(i==(PaiJuGeShu_Y-1-1)/5+1)
				zd=3*PaiJuGeShu_Y*ZhuJuGeShu_X;
			for(j=1;j<=ZhuJuGeShu_X-1;j++)
			{
				dy = dy + 1;
				if(j==BuJuGeShu_Z-5)
					break;
			}
		}
		for(k=(ZhuJuGeShu_X-1)%5+3;k<BuJuGeShu_Z-3;k+=5)//1代表允许末尾多出段数
		{
			zd=1+(ZhuJuGeShu_X-1)*PaiJuGeShu_Y+k*PaiJuGeShu_Y*ZhuJuGeShu_X+i*5;
			if(i==(PaiJuGeShu_Y-1-1)/5+1)
				zd=(ZhuJuGeShu_X-1)*PaiJuGeShu_Y+k*PaiJuGeShu_Y*ZhuJuGeShu_X+PaiJuGeShu_Y;
			for(j=1;j+k<=BuJuGeShu_Z-2;j++)
			{
				dy = dy + 1;
				if(j==ZhuJuGeShu_X-1)
					break;
			}
		}
	}
	
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	////////////////////////以下过程才开始执行整个单元建模过程//////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	dy=0;
	//横杆1/2
	element Yelement[2000];
	memset(Yelement,0,sizeof(Yelement)/sizeof(Yelement[0])*sizeof(element));
	int tempcount=1;//临时单元计数器，1开始
	for(vc=2;vc<=c+1;vc++)
	{
		for(va=1;va<=a+1;va++)
		{
			for(qd=(va-1)*b+va+vc*dc;qd<=va-1+va*b+vc*dc;qd++)
			{
				zd=qd+1;
				dy=dy+1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				Yelement[tempcount].qd=qd;
				Yelement[tempcount++].zd=zd;
				File1.WriteString(str);
				File1.WriteString("\n");
			}
		}
		
	}

	//横杆2/2
	element Xelement[2000];
	memset(Xelement,0,sizeof(Xelement)/sizeof(Xelement[0])*sizeof(element));
	tempcount=1;
	for(vc=2;vc<=c+1;vc++)
	{
		for(qd=1+dc*vc;qd<=a*(b+1)+dc*vc;qd++)
		{
			zd=qd+b+1;
			dy=dy+1;
			str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
			Xelement[tempcount].qd=qd;
			Xelement[tempcount++].zd=zd;
			File1.WriteString(str);
			File1.WriteString("\n");
		}
		
	}
	HorizPoleNum = dy;
	//立杆
	element Zelement[2000];
	memset(Zelement,0,sizeof(Zelement)/sizeof(Zelement[0])*sizeof(element));
	tempcount=1;
	for(vc=0;vc<=c+1;vc++)
	{ 
		for(qd=1+dc*vc;qd<=(a+1)*(b+1)+dc*vc;qd++)
		{
			zd=qd+dc;
			dy=dy+1;
			str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
			Zelement[tempcount].qd=qd;
			Zelement[tempcount++].zd=zd;
			File1.WriteString(str);
			File1.WriteString("\n");
		}
		
	}
	for(i=1;;i++)
	{
		if(Yelement[i].qd==0)
			break;
		for(j=1;j<jiandaoStart;j++)
		{
			if(Yelement[i].qd==NodeZong[j].Num)
			{
				Yelement[i].xs=NodeZong[j].x;
				Yelement[i].ys=NodeZong[j].y;
				Yelement[i].zs=NodeZong[j].z;
			}
			if(Yelement[i].zd==NodeZong[j].Num)
			{
				Yelement[i].xe=NodeZong[j].x;
				Yelement[i].ye=NodeZong[j].y;
				Yelement[i].ze=NodeZong[j].z;
			}
		}
	}
	for(i=1;;i++)
	{
		if(Xelement[i].qd==0)
			break;
		for(j=1;j<jiandaoStart;j++)
		{
			if(Xelement[i].qd==NodeZong[j].Num)
			{
				Xelement[i].xs=NodeZong[j].x;
				Xelement[i].ys=NodeZong[j].y;
				Xelement[i].zs=NodeZong[j].z;
			}
			if(Xelement[i].zd==NodeZong[j].Num)
			{
				Xelement[i].xe=NodeZong[j].x;
				Xelement[i].ye=NodeZong[j].y;
				Xelement[i].ze=NodeZong[j].z;
			}
		}
	}
	for(i=1;;i++)
	{
		if(Zelement[i].qd==0)
			break;
		for(j=1;j<jiandaoStart;j++)
		{
			if(Zelement[i].qd==NodeZong[j].Num)
			{
				Zelement[i].xs=NodeZong[j].x;
				Zelement[i].ys=NodeZong[j].y;
				Zelement[i].zs=NodeZong[j].z;
			}
			if(Zelement[i].zd==NodeZong[j].Num)
			{
				Zelement[i].xe=NodeZong[j].x;
				Zelement[i].ye=NodeZong[j].y;
				Zelement[i].ze=NodeZong[j].z;
			}
		}
	}
	//File1.Close();
	//AfxMessageBox("success");
	//return;
	//扫地杆
	////////////////y方向
	element SDelement[200];
	memset(SDelement,0,sizeof(SDelement)/sizeof(SDelement[0])*sizeof(element));
	tempcount=1;
	qd=saodiStart;
	for(i=1;i<PaiJuGeShu_Y;i++)
	{
		zd=qd+1;
		dy=dy+1;
		str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
		SDelement[tempcount].qd=qd;
		SDelement[tempcount++].zd=zd;
		File1.WriteString(str);
		File1.WriteString("\n");
		qd=zd;
	}
	qd=saodiStart+PaiJuGeShu_Y+2*(ZhuJuGeShu_X-2);
	for(i=1;i<PaiJuGeShu_Y;i++)
	{
		zd=qd+1;
		dy=dy+1;
		str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
		SDelement[tempcount].qd=qd;
		SDelement[tempcount++].zd=zd;
		File1.WriteString(str);
		File1.WriteString("\n");
		qd=zd;
	}
	/////////////////x方向
	qd=saodiStart;
	for(i=1;i<ZhuJuGeShu_X;i++)
	{
		if(i==1)
			zd=qd+PaiJuGeShu_Y;
		else
			zd=qd+2;
		dy=dy+1;
		str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
		SDelement[tempcount].qd=qd;
		SDelement[tempcount++].zd=zd;
		File1.WriteString(str);
		File1.WriteString("\n");
		qd=zd;
	}
	qd=saodiStart+PaiJuGeShu_Y-1;
	for(i=1;i<ZhuJuGeShu_X;i++)
	{
		if(i==ZhuJuGeShu_X-1)
			zd=qd+PaiJuGeShu_Y;
		else
			zd=qd+2;
		dy=dy+1;
		str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
		SDelement[tempcount].qd=qd;
		SDelement[tempcount++].zd=zd;
		File1.WriteString(str);
		File1.WriteString("\n");
		qd=zd;
	}
	tempcount=1;
	for(i=0;i<countline;i++)
	{
		float tempres[2];
		for(j=1;;j++)
		{//写算法判断两单元是否相交，并返回交点
			if(Zelement[j].qd==0)
				break;
			tempres[0]=0.0;
			tempres[1]=0.0;
			JudgeCross(Zelement[j].xs,Zelement[j].zs,Zelement[j].xe,Zelement[j].ze,
				XZline[0][i].xs,XZline[0][i].zs,XZline[0][i].xe,XZline[0][i].ze,tempres);
			if(tempres[0]==-1.0)
				continue;
			int flag=0;
			for(k=1;k<tempcount;k++)
			{
				if((XZNode[k].x==tempres[0])&&(XZNode[k].z==tempres[1]))
				{
					flag=1;
					break;
				}
			}
			if(0==flag)
			{
				XZNode[tempcount].x=tempres[0];
				XZNode[tempcount].z=tempres[1];
				XZNode[tempcount].Num=num+tempcount;
				tempcount++;
			}
		}
	}
		str.Format(";///////////////////////////////////////我是分割线\n");
		File1.WriteString(str);
	for(i=1;i<tempcount;i++)
	{
		str.Format("%d,  %7.2f , %7.2f , %7.2f",XZNode[i].Num,XZNode[i].x,XZNode[i].y,XZNode[i].z);
		File1.WriteString(str);
		File1.WriteString("\n");
	}
		str.Format(";///////////////////////////////////////我是分割线\n");
		File1.WriteString(str);
	
	
	element XZscielement[2000];
	memset(XZscielement,0,sizeof(XZscielement)/sizeof(XZscielement[0])*sizeof(element));
	Node tempNode[50];
	memset(tempNode,0,sizeof(tempNode)/sizeof(tempNode[0])*sizeof(Node));
	Node tempNode1[50];
	memset(tempNode1,0,sizeof(tempNode1)/sizeof(tempNode1[0])*sizeof(Node));

	int Node1count=1;
	int scicount=1;
	/*Node tempstart;
	tempcount=1; 
	tempNode.y=0.0;
	tempstart.y=0.0;*/
	for(i=0;i<countline;i++)
	{
		tempcount=1;
		memset(tempNode,0,sizeof(tempNode)/sizeof(tempNode[0])*sizeof(Node));
		for(j=1;;j++)
		{
			if(XZNode[j].Num==0)
				break;
			float res=(XZNode[j].x-XZline[0][i].xs)*(XZline[0][i].zs-XZline[0][i].ze)/(XZline[0][i].xs-XZline[0][i].xe)+XZline[0][i].zs-XZNode[j].z;
			if(fabs(res)<=0.000002&&(XZNode[j].z-XZline[0][i].ze)*(XZNode[j].z-XZline[0][i].zs)<=0.0)
			{
				tempNode[tempcount++]=XZNode[j];
			}
		}//此处按顺序找到所有应该连线的剪刀撑坐标，待连接，睡了。。。
		Node tempsingleNode;
		float tempz=tempNode[1].z;
		Node1count=1;
		memset(tempNode1,0,sizeof(tempNode1)/sizeof(tempNode1[0])*sizeof(Node));
		float tempmin=0.0;
		for(j=1;j<tempcount;j++)
		{
			if(j==1)
				tempmin=-1.0;
			else
				tempmin=tempz;
			tempz=200.0;
			for(k=1;k<tempcount;k++)
			{
				if(tempNode[k].z>tempmin)
				{
					if(tempNode[k].z<tempz)
					{
						tempsingleNode=tempNode[k];
						tempz=tempNode[k].z;
					}
				}
			}
			tempNode1[Node1count++]=tempsingleNode;
		}
		for(j=1;j<Node1count-1;j++)
		{
			XZscielement[scicount].qd=tempNode1[j].Num;
			XZscielement[scicount].xs=tempNode1[j].x;
			XZscielement[scicount].ys=tempNode1[j].y;
			XZscielement[scicount].zs=tempNode1[j].z;
			XZscielement[scicount].zd=tempNode1[j+1].Num;
			XZscielement[scicount].xe=tempNode1[j+1].x;
			XZscielement[scicount].ye=tempNode1[j+1].y;
			XZscielement[scicount++].ze=tempNode1[j+1].z;
		}
	}
	for(i=1;i<scicount;i++)
	{
		//dy=dy+1;
		str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d\n",i+dy,"BEAM",1,1,XZscielement[i].qd,XZscielement[i].zd,0);
		//Yelement[tempcount].qd=qd;
		//Yelement[tempcount++].zd=zd;
		File1.WriteString(str);
	}
	str.Format("scicount=%d",scicount-1);
	AfxMessageBox(str);
	File1.Close();
	AfxMessageBox("success");
	return;

	/*for(i=0;i<countline;i++)
	{
		int temps=0,tempe=0;
		for(j=1;j<=num;j++)
		{
			if((NodeZong[j].y=0.0)&&(NodeZong[j].x==XZline[0][i].xs)&&(NodeZong[j].z==XZline[0][i].zs))
			{
				temps=NodeZong[j].Num;
			}
			if((NodeZong[j].y=0.0)&&(NodeZong[j].x==XZline[0][i].xe)&&(NodeZong[j].z==XZline[0][i].ze))
			{
				tempe=NodeZong[j].Num;
			}
		}
		if(temps!=0&&tempe!=0)
		{
		dy=dy+1;
		str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,temps,tempe,0);
		File1.WriteString(str);
		File1.WriteString("\n");
		}
	}*/
	//str.Format("countline=%d,tempZ-%.2f,tempY-%.2f,tempX-%.2f",countline,tempZ,tempY,tempX);
	//AfxMessageBox(str);
	File1.Close();
	AfxMessageBox("success");
	return;
	/*int ZCount=0;
	int ZNum[20];
	float tempZDate=0.0;
	int tempZNum=2;
	ZNum[ZCount++]=1+ZhuJuGeShu_X*PaiJuGeShu_Y*tempZNum;
	for(i=2;i<Count_Z-1;i++)
	{
	for(j=0;j<BuJuCountSave[i];j++)
	{
	tempZDate=tempZDate+BuJuDataSave[i];
	if(tempZDate>=4.8)
	{
		ZNum[ZCount++]=1+ZhuJuGeShu_X*PaiJuGeShu_Y*tempZNum;
		tempZDate=BuJuDataSave[i];
		}
		tempZNum++;
		}
		}
		if(ZNum[ZCount-1]!=1+ZhuJuGeShu_X*PaiJuGeShu_Y*(BuJuGeShu_Z-1))
		{
		ZNum[ZCount++]=1+ZhuJuGeShu_X*PaiJuGeShu_Y*(BuJuGeShu_Z-2);
	}*/
	//////////////////////水平剪刀撑///////////////////////////
	for(i=0;i<ZCount;i++)
	{
		//int Max=ZhuJuGeShu_X>PaiJuGeShu_Y?ZhuJuGeShu_X:PaiJuGeShu_Y;
		//int Min=ZhuJuGeShu_X<PaiJuGeShu_Y?ZhuJuGeShu_X:PaiJuGeShu_Y;
		for(k=0;k<=(PaiJuGeShu_Y-1-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=ZNum[i]+k*5;
			for(j=1+5*k;j<=PaiJuGeShu_Y-1;j++)
			{
				qd=zd;
				zd=zd+PaiJuGeShu_Y+1;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if((j-5*k)==ZhuJuGeShu_X-1)
					break;
			}
		}
		for(k=1;k<=(ZhuJuGeShu_X-1-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=ZNum[i]+k*5*PaiJuGeShu_Y;
			for(j=1+5*k;j<=ZhuJuGeShu_X-1;j++)
			{
				qd=zd;
				zd=zd+PaiJuGeShu_Y+1;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if((j-5*k)==PaiJuGeShu_Y-1)
					break;
			}
		}
		for(k=1;k<=(ZhuJuGeShu_X-1-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=ZNum[i]+k*5*PaiJuGeShu_Y;
			for(j=1;j<=5*k;j++)
			{
				qd=zd;
				zd=zd-PaiJuGeShu_Y+1;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if(j==PaiJuGeShu_Y-1)
					break;
			}
		}
		if((ZhuJuGeShu_X-1)%5>=3)
		{
			zd=ZNum[i]+ZhuJuGeShu_X*PaiJuGeShu_Y-PaiJuGeShu_Y;
			for(j=1;j<=PaiJuGeShu_Y-1;j++)
			{
				qd=zd;
				zd=zd-PaiJuGeShu_Y+1;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if(j==ZhuJuGeShu_X-1)
					break;
			}
		}
		for(k=(ZhuJuGeShu_X-1)%5;k<PaiJuGeShu_Y-1;k+=5)//1代表允许末尾多出段数
		{
			zd=ZNum[i]+ZhuJuGeShu_X*PaiJuGeShu_Y-PaiJuGeShu_Y+k;
			for(j=1;j<=ZhuJuGeShu_X-1;j++)
			{
				qd=zd;
				zd=zd-PaiJuGeShu_Y+1;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if(j+k==PaiJuGeShu_Y-1)
					break;
			}
		}
	}
	//File1.Close();
	//AfxMessageBox("success");
	//return;
	//File1.WriteString(";;横桥向剪刀撑\n");
	
	//////////////////////////////////////////////////////////
	///////////////////////横桥向剪刀撑//////////////////////////////
	for(i=0;i<=(ZhuJuGeShu_X-1-1)/5+1;i++)
	{
		//int Max=ZhuJuGeShu_X>PaiJuGeShu_Y?ZhuJuGeShu_X:PaiJuGeShu_Y;
		//int Min=ZhuJuGeShu_X<PaiJuGeShu_Y?ZhuJuGeShu_X:PaiJuGeShu_Y;
		for(k=0;k<=(PaiJuGeShu_Y-1-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=1+2*ZhuJuGeShu_X*PaiJuGeShu_Y+i*5*PaiJuGeShu_Y+k*5;
			if(i==(ZhuJuGeShu_X-1-1)/5+1)
				zd=1+2*ZhuJuGeShu_X*PaiJuGeShu_Y+(ZhuJuGeShu_X-1)*PaiJuGeShu_Y+k*5;
			for(j=1;j<=BuJuGeShu_Z-5;j++)
			{
				qd=zd;
				zd=zd+PaiJuGeShu_Y*ZhuJuGeShu_X*(j==1?2:1)+1;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if((j+5*k)==PaiJuGeShu_Y-1)
					break;
			}
		}
		for(k=1;k<=(BuJuGeShu_Z-5)/5;k++)//1代表允许末尾多出段数
		{
			zd=1+3*ZhuJuGeShu_X*PaiJuGeShu_Y+i*5*PaiJuGeShu_Y+k*ZhuJuGeShu_X*PaiJuGeShu_Y*5;
			if(i==(ZhuJuGeShu_X-1-1)/5+1)
				zd=1+3*ZhuJuGeShu_X*PaiJuGeShu_Y+(ZhuJuGeShu_X-1)*PaiJuGeShu_Y+k*ZhuJuGeShu_X*PaiJuGeShu_Y*5;
			for(j=1;j<=PaiJuGeShu_Y-1;j++)
			{
				qd=zd;
				zd=zd+PaiJuGeShu_Y*ZhuJuGeShu_X+1;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if((j+5*k)==BuJuGeShu_Z-5)
					break;
			}
		}
		/////////////////////////////0915暂停
		for(k=1;k<=(PaiJuGeShu_Y-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=1+2*ZhuJuGeShu_X*PaiJuGeShu_Y+i*5*PaiJuGeShu_Y+k*5;
			if(i==(ZhuJuGeShu_X-1-1)/5+1)
				zd=1+2*ZhuJuGeShu_X*PaiJuGeShu_Y+(ZhuJuGeShu_X-1)*PaiJuGeShu_Y+k*5;
			for(j=1;j<=5*k;j++)
			{
				qd=zd;
				zd=zd+PaiJuGeShu_Y*ZhuJuGeShu_X*(j==1?2:1)-1;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if(j==BuJuGeShu_Z-5)
					break;
			}
		}
		if((PaiJuGeShu_Y-1)%5>=3)
		{
			zd=2*ZhuJuGeShu_X*PaiJuGeShu_Y+i*5*PaiJuGeShu_Y+PaiJuGeShu_Y;
			if(i==(ZhuJuGeShu_X-1-1)/5+1)
				zd=3*ZhuJuGeShu_X*PaiJuGeShu_Y;
			for(j=1;j<=PaiJuGeShu_Y-1;j++)
			{
				qd=zd;
				zd=zd+PaiJuGeShu_Y*ZhuJuGeShu_X*(j==1?2:1)-1;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if(j==BuJuGeShu_Z-5)
					break;
			}
		}
		for(k=(PaiJuGeShu_Y-1)%5+3;k<BuJuGeShu_Z-3;k+=5)//1代表允许末尾多出段数
		{
			zd=PaiJuGeShu_Y+k*ZhuJuGeShu_X*PaiJuGeShu_Y+i*5*PaiJuGeShu_Y;
			if(i==(ZhuJuGeShu_X-1-1)/5+1)
				zd=PaiJuGeShu_Y+k*ZhuJuGeShu_X*PaiJuGeShu_Y+(ZhuJuGeShu_X-1)*PaiJuGeShu_Y;
			for(j=1;j+k<=BuJuGeShu_Z-2;j++)
			{
				qd=zd;
				zd=zd+PaiJuGeShu_Y*ZhuJuGeShu_X-1;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if(j==PaiJuGeShu_Y-1)
					break;
			}
		}
	}
	
	///////////////////////顺桥向剪刀撑//////////////////////////////
	for(i=0;i<=(PaiJuGeShu_Y-1-1)/5+1;i++)
	{
		//int Max=PaiJuGeShu_Y>ZhuJuGeShu_X?PaiJuGeShu_Y:ZhuJuGeShu_X;
		//int Min=PaiJuGeShu_Y<ZhuJuGeShu_X?PaiJuGeShu_Y:ZhuJuGeShu_X;
		for(k=0;k<=(ZhuJuGeShu_X-1-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=1+2*PaiJuGeShu_Y*ZhuJuGeShu_X+i*5+k*5*PaiJuGeShu_Y;
			if(i==(PaiJuGeShu_Y-1-1)/5+1)
				zd=1+2*PaiJuGeShu_Y*ZhuJuGeShu_X+PaiJuGeShu_Y-1+k*5*PaiJuGeShu_Y;
			for(j=1;j<=BuJuGeShu_Z-5;j++)
			{
				qd=zd;
				zd=zd+ZhuJuGeShu_X*PaiJuGeShu_Y*(j==1?2:1)+PaiJuGeShu_Y;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if((j+5*k)==ZhuJuGeShu_X-1)
					break;
			}
		}
		for(k=1;k<=(BuJuGeShu_Z-5)/5;k++)//1代表允许末尾多出段数
		{
			zd=1+3*PaiJuGeShu_Y*ZhuJuGeShu_X+i*5+k*5*PaiJuGeShu_Y*ZhuJuGeShu_X;
			if(i==(PaiJuGeShu_Y-1-1)/5+1)
				zd=3*PaiJuGeShu_Y*ZhuJuGeShu_X+PaiJuGeShu_Y+k*ZhuJuGeShu_X*PaiJuGeShu_Y*5;
			for(j=1;j<=ZhuJuGeShu_X-1;j++)
			{
				qd=zd;
				zd=zd+ZhuJuGeShu_X*PaiJuGeShu_Y+PaiJuGeShu_Y;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if((j+5*k)==BuJuGeShu_Z-5)
					break;
			}
		}
		/////////////////////////////0915暂停
		for(k=1;k<=(ZhuJuGeShu_X-1)/5;k++)//1代表允许末尾多出段数
		{
			zd=1+2*PaiJuGeShu_Y*ZhuJuGeShu_X+i*5+k*5*PaiJuGeShu_Y;
			if(i==(PaiJuGeShu_Y-1-1)/5+1)
				zd=2*PaiJuGeShu_Y*ZhuJuGeShu_X+PaiJuGeShu_Y+k*5*PaiJuGeShu_Y;
			for(j=1;j<=5*k;j++)
			{
				qd=zd;
				zd=zd+ZhuJuGeShu_X*PaiJuGeShu_Y*(j==1?2:1)-PaiJuGeShu_Y;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if(j==BuJuGeShu_Z-5)
					break;
			}
		}
		if((ZhuJuGeShu_X-1)%5>=3)
		{
			zd=1+2*PaiJuGeShu_Y*ZhuJuGeShu_X+i*5+(ZhuJuGeShu_X-1)*PaiJuGeShu_Y;
			if(i==(PaiJuGeShu_Y-1-1)/5+1)
				zd=3*PaiJuGeShu_Y*ZhuJuGeShu_X;
			for(j=1;j<=ZhuJuGeShu_X-1;j++)
			{
				qd=zd;
				zd=zd+ZhuJuGeShu_X*PaiJuGeShu_Y*(j==1?2:1)-PaiJuGeShu_Y;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if(j==BuJuGeShu_Z-5)
					break;
			}
		}
		for(k=(ZhuJuGeShu_X-1)%5+3;k<BuJuGeShu_Z-3;k+=5)//1代表允许末尾多出段数
		{
			zd=1+(ZhuJuGeShu_X-1)*PaiJuGeShu_Y+k*PaiJuGeShu_Y*ZhuJuGeShu_X+i*5;
			if(i==(PaiJuGeShu_Y-1-1)/5+1)
				zd=(ZhuJuGeShu_X-1)*PaiJuGeShu_Y+k*PaiJuGeShu_Y*ZhuJuGeShu_X+PaiJuGeShu_Y;
			for(j=1;j+k<=BuJuGeShu_Z-2;j++)
			{
				qd=zd;
				zd=zd+ZhuJuGeShu_X*PaiJuGeShu_Y-PaiJuGeShu_Y;
				dy = dy + 1;
				str.Format("%d , %s ,    %d,    %d,    %d,    %d,    %d",dy,"BEAM",1,1,qd,zd,0);
				File1.WriteString(str);
				File1.WriteString("\n");
				if(j==ZhuJuGeShu_X-1)
					break;
			}
		}
	}
	
	
	
	qy = 1.1 * (GangJinHunNingTu + MoBanFangLeng);
	str.Format("%f",qy);
	
	if (DiYiCiYuYa + DiErCiYuYa + DiSanCiYuYa > 1.4) //Then
	{
		AfxMessageBox("预压荷载是否过大？");
	}
	//************Lines = Lines & readFile("d:\参数化建模\支架文件\GROUP.mct")**********//
	
	str.Format("*GROUP    ; Group; NAME, NODE_LIST, ELEM_LIST, PLANE_TYPE",1,1,qd,zd,0);
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	//*******  Lines = Lines & "横桥向横杆" & "," & 1 & "to" & m1 & vbCrLf  ***********
	//ZhuJuGeShu_X*PaiJuGeShu_Y*(BuJuGeShu_Z
	m1=(PaiJuGeShu_Y-1)*ZhuJuGeShu_X*(BuJuGeShu_Z-3);
	str.Format("%s ,,   %d   %s   %d","横桥向横杆",1,"to",m1);
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	//************Lines = Lines & "顺桥向横杆" & "," & m1 + 1 & "to" & m2 & vbCrLf *******
	m2=m1+(ZhuJuGeShu_X-1)*PaiJuGeShu_Y*(BuJuGeShu_Z-3);
	str.Format("%s ,,    %d   %s   %d","顺桥向横杆",m1+1,"to",m2);
	File1.WriteString(str);
	File1.WriteString("\n");
	
	//************Lines = Lines & "立杆" & "," & m2 + 1 & "to" & m3 & vbCrLf *************
	m3=m2+ZhuJuGeShu_X*PaiJuGeShu_Y*(BuJuGeShu_Z-1);
	str.Format("%s ,,    %d   %s   %d","立杆",m2+1,"to",m3);
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	//*********** Lines = Lines & "剪刀撑" & "," & m3 + 1 & "to" & dy & vbCrLf **********
	
	str.Format("%s ,,    %d   %s   %d","剪刀撑",m3+1,"to",dy);
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	//'边界组、荷载组定义
	//************* Lines = Lines & readFile("d:\参数化建模\支架文件\BNDR-GROUP.mct") ************
	
	str=_T("*BNDR-GROUP    ; Boundary Group");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NAME");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("地基支撑\n铰接\n横杆-立杆\n剪刀撑-立杆");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*LOAD-GROUP    ; Load Group");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NAME");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("预压第一次");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("预压第二次");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("预压第三次");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("浇筑第一次");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("浇筑第二次");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("风荷载");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("支架自重");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("模板方楞等");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("附加构件");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("施工人员机械");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("浇筑及振捣");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	//'材料特性、截面定义
	//*******  Lines = Lines & readFile("d:\参数化建模\支架文件\MATERIAL.mct") ************
	str=_T("*MATERIAL    ; Material");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iMAT, TYPE, MNAME, SPHEAT, HEATCO, PLAST, TUNIT, bMASS, DAMPRATIO, [DATA1]          ; STEEL, CONC, USER");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iMAT, TYPE, MNAME, SPHEAT, HEATCO, PLAST, TUNIT, bMASS, DAMPRATIO, [DATA2], [DATA2] ; SRC");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DATA1] : 1, DB, NAME, CODE ");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DATA1] : 2, ELAST, POISN, THERMAL, DEN, MASS");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DATA1] : 3, Ex, Ey, Ez, Tx, Ty, Tz, Sxy, Sxz, Syz, Pxy, Pxz, Pyz, DEN, MASS   ; Orthotropic");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DATA2] : 1, DB, NAME, CODE or 2, ELAST, POISN, THERMAL, DEN, MASS");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("    1, STEEL, Q235              , 0, 0, , C, NO, 0.02, 1, GB03(S)    ,            , Q235  ");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	
	str=_T("*MATL-COLOR");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iMAT, W_R, W_G, W_B, HF_R, HF_G, HF_B, HE_R, HE_G, HE_B, bBLEND, FACT");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("    1, 255,   0,   0,    0, 255,   0,    0,   0, 255,  NO, 0.5");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	File1.WriteString("\n");
	
	
	str=_T("*SECTION    ; Section");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE, [DATA1], [DATA2]                    ; 1st line - DB/USER");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE, BLT, D1, ..., D8, iCEL              ; 1st line - VALUE");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       AREA, ASy, ASz, Ixx, Iyy, Izz                                          ; 2nd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       CyP, CyM, CzP, CzM, QyB, QzB, PERI_OUT, PERI_IN, Cy, Cz                ; 3rd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       Y1, Y2, Y3, Y4, Z1, Z2, Z3, Z4, Zyy, Zzz                               ; 4th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE, ELAST, DEN, POIS, POIC, SF          ; 1st line - SRC");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       D1, D2, [SRC]                                                          ; 2nd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE, 1, DB, NAME1, NAME2, D1, D2         ; 1st line - COMBINED");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE, 2, D11, D12, D13, D14, D15, D21, D22, D23, D24");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET2], bSD, SHAPE, iyVAR, izVAR, STYPE                ; 1st line - TAPERED");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       DB, NAME1, NAME2                                                       ; 2nd line(STYPE=DB)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [DIM1], [DIM2]                                                         ; 2nd line(STYPE=USER)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       D11, D12, D13, D14, D15, D16, D17, D18                                 ; 2nd line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       AREA1, ASy1, ASz1, Ixx1, Iyy1, Izz1                                    ; 3rd line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       CyP1, CyM1, CzP1, CzM1, QyB1, QzB1, PERI_OUT1, PERI_IN1, Cy1, Cz1      ; 4th line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       Y11, Y12, Y13, Y14, Z11, Z12, Z13, Z14, Zyy1, Zyy2                     ; 5th line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       D21, D22, D23, D24, D25, D26, D27, D28                                 ; 6th line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       AREA2, ASy2, ASz2, Ixx2, Iyy2, Izz2                                    ; 7th line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       CyP2, CyM2, CzP2, CzM2, QyB2, QzB2, PERI_OUT2, PERI_IN2, Cy2, Cz2      ; 8th line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       Y21, Y22, Y23, Y24, Z21, Z22, Z23, Z24, Zyy2, Zzz2                     ; 9th line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       OPT1, OPT2, [JOINT]                                                    ; 2nd line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       ELAST, DEN, POIS, POIC                                                 ; 2nd line(STYPE=PSC-CMPW)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       bSHEARCHK, [SCHK-I], [SCHK-J], [WT-I], [WT-J], WI, WJ, bSYM, bSIDEHOLE ; 3rd line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       bSHEARCHK, bSYM, bHUNCH, [CMPWEB-I], [CMPWEB-J]                        ; 3rd line(STYPE=PSC-CMPW)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       bUSERDEFMESHSIZE, MESHSIZE, bUSERINPSTIFF, [STIFF-I], [STIFF-J]        ; 4th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-A]-i                                                             ; 5th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-B]-i                                                             ; 6th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-C]-i                                                             ; 7th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-D]-i                                                             ; 8th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-A]-j                                                             ; 9th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-B]-j                                                             ; 10th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-C]-j                                                             ; 11th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-D]-j                                                             ; 12th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       GN, CTC, Bc, Tc, Hh, EsEc, DsDc, Ps, Pc, bMULTI, EsEc-L, EsEc-S        ; 2nd line(STYPE=CMP-B/I)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SW_i, Hw_i, tw_i, B_i, Bf1_i, tf1_i, B2_i, Bf2_i, tf2_i                ; 3rd line(STYPE=CMP-B/I)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SW_j, Hw_j, tw_j, B_j, Bf1_j, tf1_j, B2_j, Bf2_j, tf2_j                ; 4th line(STYPE=CMP-B/I)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       N1, N2, Hr, Hr2, tr1, tr2                                              ; 5th line(STYPE=CMP-B)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       GN, CTC, Bc, Tc, Hh, EgdEsb, DgdDsb, Pgd, Psb, bSYM, SW_i, SW_j        ; 2nd line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       OPT1, OPT2, [JOINT]                                                    ; 3rd line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-A]-i                                                             ; 4th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-B]-i                                                             ; 5th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-C]-i                                                             ; 6th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-D]-i                                                             ; 7th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-A]-j                                                             ; 8th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-B]-j                                                             ; 9th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-C]-j                                                             ; 10th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-D]-j                                                             ; 11th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, STYPE1, STYPE2                             ; 1st line - CONSTRUCT");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SHAPE, ...(same with other type data from shape)                       ; Before (STYPE1)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SHAPE, ...(same with other type data from shape)                       ; After  (STYPE2)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE                                      ; 1st line - COMPOSITE-SB");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       Hw, tw, B, Bf1, tf1, B2, Bf2, tf2                                      ; 2nd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       N1, N2, Hr, Hr2, tr1, tr2                                              ; 3rd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SW, GN, CTC, Bc, Tc, Hh, EsEc, DsDc, Ps, Pc, bMulti, Elong, Esh        ; 4th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE                                      ; 1st line - COMPOSITE-SI");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       Hw, tw, B, tf1, B2, tf2                                                ; 2nd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SW, GN, CTC, Bc, Tc, Hh, EsEc, DsDc, Ps, Pc, bMulti, Elong, Esh        ; 3rd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE                                      ; 1st line - COMPOSITE-CI/CT");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       OPT1, OPT2, [JOINT]                                                    ; 2nd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-A]                                                               ; 3rd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-B]                                                               ; 4th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-C]                                                               ; 5th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-D]                                                               ; 6th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SW, GN, CTC, Bc, Tc, Hh, EgdEsb, DgdDsb, Pgd, Psb                      ; 7th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE                                      ; 1st line - PSC");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       OPT1, OPT2, [JOINT]                                                    ; 2nd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       bSHEARCHK, [SCHK], [WT], WIDTH, bSYM, bSIDEHOLE                        ; 3rd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       bUSERDEFMESHSIZE, MESHSIZE, bUSERINPSTIFF, [STIFF]                     ; 4th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-A]                                                               ; 5th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-B]                                                               ; 6th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-C]                                                               ; 7th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-D]                                                               ; 8th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DATA1] : 1, DB, NAME or 2, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DATA2] : CCSHAPE or iCEL or iN1, iN2");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [SRC]  : 1, DB, NAME1, NAME2 or 2, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, iN1, iN2");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DIM1], [DIM2] : D1, D2, D3, D4, D5, D6, D7, D8");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [OFFSET] : OFFSET, iCENT, iREF, iHORZ, HUSER, iVERT, VUSER");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [OFFSET2]: OFFSET, iCENT, iREF, iHORZ, HUSERI, HUSERJ, iVERT, VUSERI, VUSERJ");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [JOINT]  :  8(1CELL, 2CELL), 13(3CELL),  9(PSCM),  8(PSCH), 9(PSCT),  2(PSCB), 0(nCELL),  2(nCEL2)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [SIZE-A] :  6(1CELL, 2CELL), 10(3CELL), 10(PSCM),  6(PSCH), 8(PSCT), 10(PSCB), 5(nCELL), 11(nCEL2)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [SIZE-B] :  6(1CELL, 2CELL), 12(3CELL),  6(PSCM),  6(PSCH), 8(PSCT),  6(PSCB), 8(nCELL), 18(nCEL2)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [SIZE-C] : 10(1CELL, 2CELL), 13(3CELL),  9(PSCM), 10(PSCH), 7(PSCT),  8(PSCB), 0(nCELL), 11(nCEL2)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [SIZE-D] :  8(1CELL, 2CELL), 13(3CELL),  6(PSCM),  7(PSCH), 8(PSCT),  5(PSCB), 0(nCELL), 18(nCEL2)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [STIFF]  : AREA, ASy, ASz, Ixx, Iyy, Izz");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [SCHK]   : bAUTO_Z1, Z1, bAUTO_Z3, Z3");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [WT]     : bAUTO_TOR, TOR, bAUTO_SHR1, SHR1, bAUTO_SHR2, SHR2, bAUTO_SHR3, SHR3");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [CMPWEB] : EFD, LRF, A, B, H, T");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	File1.WriteString("\n");
	CString tempstr="";
	((CEdit*)GetDlgItem(IDE_GangGuanWaiJing))->GetWindowText(tempstr);
	D=atof(tempstr);
	D_Deviation[1]=D;
	D_Deviation_Per[1]=100;
	((CEdit*)GetDlgItem(IDE_GangGuanBiHou))->GetWindowText(tempstr);
	tw=atof(tempstr);
	tw_Deviation[1]=tw;
	tw_Deviation_Per[1]=100;
	int SectionCount=1;
	for(i=1;i<=D_Deviation_Per[0];i++)
	{
		for(j=1;j<=tw_Deviation_Per[0];j++)
		{
			str.Format("    %d, DBUSER    , 钢管截面          , CC, 0, 0, 0, 0, 0, 0, YES, P  , 2, %.3f, %.4f, 0, 0, 0, 0, 0, 0, 0, 0\n\n",SectionCount++,D_Deviation[i],tw_Deviation[j]);
			File1.WriteString(str);
		}
	}
	File1.WriteString("\n");
	File1.WriteString("\n");
	
	str=_T("*SECT-COLOR");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, W_R, W_G, W_B, HF_R, HF_G, HF_B, HE_R, HE_G, HE_B, bBLEND, FACT");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("    1, 255,   0,   0,    0, 255,   0,    0,   0, 255,  NO, 0.5");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	
	str=_T("*DGN-SECT");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE, [DATA1], [DATA2]                    ; 1st line - DB/USER");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE, BLT, D1, ..., D8, iCEL              ; 1st line - VALUE");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       AREA, ASy, ASz, Ixx, Iyy, Izz                                          ; 2nd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       CyP, CyM, CzP, CzM, QyB, QzB, PERI_OUT, PERI_IN, Cy, Cz                ; 3rd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       Y1, Y2, Y3, Y4, Z1, Z2, Z3, Z4, Zyy, Zzz                               ; 4th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE, ELAST, DEN, POIS, POIC, SF          ; 1st line - SRC");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       D1, D2, [SRC]                                                          ; 2nd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE, 1, DB, NAME1, NAME2, D1, D2         ; 1st line - COMBINED");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE, 2, D11, D12, D13, D14, D15, D21, D22, D23, D24");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET2], bSD, SHAPE, iyVAR, izVAR, STYPE                ; 1st line - TAPERED");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       DB, NAME1, NAME2                                                       ; 2nd line(STYPE=DB)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [DIM1], [DIM2]                                                         ; 2nd line(STYPE=USER)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       D11, D12, D13, D14, D15, D16, D17, D18                                 ; 2nd line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       AREA1, ASy1, ASz1, Ixx1, Iyy1, Izz1                                    ; 3rd line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       CyP1, CyM1, CzP1, CzM1, QyB1, QzB1, PERI_OUT1, PERI_IN1, Cy1, Cz1      ; 4th line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       Y11, Y12, Y13, Y14, Z11, Z12, Z13, Z14, Zyy1, Zyy2                     ; 5th line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       D21, D22, D23, D24, D25, D26, D27, D28                                 ; 6th line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       AREA2, ASy2, ASz2, Ixx2, Iyy2, Izz2                                    ; 7th line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       CyP2, CyM2, CzP2, CzM2, QyB2, QzB2, PERI_OUT2, PERI_IN2, Cy2, Cz2      ; 8th line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       Y21, Y22, Y23, Y24, Z21, Z22, Z23, Z24, Zyy2, Zzz2                     ; 9th line(STYPE=VALUE)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       OPT1, OPT2, [JOINT]                                                    ; 2nd line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       ELAST, DEN, POIS, POIC                                                 ; 2nd line(STYPE=PSC-CMPW)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       bSHEARCHK, [SCHK-I], [SCHK-J], [WT-I], [WT-J], WI, WJ, bSYM, bSIDEHOLE ; 3rd line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       bSHEARCHK, bSYM, bHUNCH, [CMPWEB-I], [CMPWEB-J]                        ; 3rd line(STYPE=PSC-CMPW)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       bUSERDEFMESHSIZE, MESHSIZE, bUSERINPSTIFF, [STIFF-I], [STIFF-J]        ; 4th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-A]-i                                                             ; 5th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-B]-i                                                             ; 6th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-C]-i                                                             ; 7th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-D]-i                                                             ; 8th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-A]-j                                                             ; 9th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-B]-j                                                             ; 10th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-C]-j                                                             ; 11th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-D]-j                                                             ; 12th line(STYPE=PSC)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       GN, CTC, Bc, Tc, Hh, EsEc, DsDc, Ps, Pc, bMULTI, EsEc-L, EsEc-S        ; 2nd line(STYPE=CMP-B/I)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SW_i, Hw_i, tw_i, B_i, Bf1_i, tf1_i, B2_i, Bf2_i, tf2_i                ; 3rd line(STYPE=CMP-B/I)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SW_j, Hw_j, tw_j, B_j, Bf1_j, tf1_j, B2_j, Bf2_j, tf2_j                ; 4th line(STYPE=CMP-B/I)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       N1, N2, Hr, Hr2, tr1, tr2                                              ; 5th line(STYPE=CMP-B)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       GN, CTC, Bc, Tc, Hh, EgdEsb, DgdDsb, Pgd, Psb, bSYM, SW_i, SW_j        ; 2nd line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       OPT1, OPT2, [JOINT]                                                    ; 3rd line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-A]-i                                                             ; 4th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-B]-i                                                             ; 5th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-C]-i                                                             ; 6th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-D]-i                                                             ; 7th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-A]-j                                                             ; 8th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-B]-j                                                             ; 9th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-C]-j                                                             ; 10th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-D]-j                                                             ; 11th line(STYPE=CMP-CI/CT)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, STYPE1, STYPE2                             ; 1st line - CONSTRUCT");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SHAPE, ...(same with other type data from shape)                       ; Before (STYPE1)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SHAPE, ...(same with other type data from shape)                       ; After  (STYPE2)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE                                      ; 1st line - COMPOSITE-SB");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       Hw, tw, B, Bf1, tf1, B2, Bf2, tf2                                      ; 2nd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       N1, N2, Hr, Hr2, tr1, tr2                                              ; 3rd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SW, GN, CTC, Bc, Tc, Hh, EsEc, DsDc, Ps, Pc, bMulti, Elong, Esh        ; 4th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE                                      ; 1st line - COMPOSITE-SI");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       Hw, tw, B, tf1, B2, tf2                                                ; 2nd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SW, GN, CTC, Bc, Tc, Hh, EsEc, DsDc, Ps, Pc, bMulti, Elong, Esh        ; 3rd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE                                      ; 1st line - COMPOSITE-CI/CT");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       OPT1, OPT2, [JOINT]                                                    ; 2nd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-A]                                                               ; 3rd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-B]                                                               ; 4th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-C]                                                               ; 5th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-D]                                                               ; 6th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       SW, GN, CTC, Bc, Tc, Hh, EgdEsb, DgdDsb, Pgd, Psb                      ; 7th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iSEC, TYPE, SNAME, [OFFSET], bSD, SHAPE                                      ; 1st line - PSC");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       OPT1, OPT2, [JOINT]                                                    ; 2nd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       bSHEARCHK, [SCHK], [WT], WIDTH, bSYM, bSIDEHOLE                        ; 3rd line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       bUSERDEFMESHSIZE, MESHSIZE, bUSERINPSTIFF, [STIFF]                     ; 4th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-A]                                                               ; 5th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-B]                                                               ; 6th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-C]                                                               ; 7th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";       [SIZE-D]                                                               ; 8th line");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DATA1] : 1, DB, NAME or 2, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DATA2] : CCSHAPE or iCEL or iN1, iN2");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [SRC]  : 1, DB, NAME1, NAME2 or 2, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, iN1, iN2");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DIM1], [DIM2] : D1, D2, D3, D4, D5, D6, D7, D8");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [OFFSET] : OFFSET, iCENT, iREF, iHORZ, HUSER, iVERT, VUSER");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [OFFSET2]: OFFSET, iCENT, iREF, iHORZ, HUSERI, HUSERJ, iVERT, VUSERI, VUSERJ");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [JOINT]  :  8(1CELL, 2CELL), 13(3CELL),  9(PSCM),  8(PSCH), 9(PSCT),  2(PSCB), 0(nCELL),  2(nCEL2)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [SIZE-A] :  6(1CELL, 2CELL), 10(3CELL), 10(PSCM),  6(PSCH), 8(PSCT), 10(PSCB), 5(nCELL), 11(nCEL2)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [SIZE-B] :  6(1CELL, 2CELL), 12(3CELL),  6(PSCM),  6(PSCH), 8(PSCT),  6(PSCB), 8(nCELL), 18(nCEL2)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [SIZE-C] : 10(1CELL, 2CELL), 13(3CELL),  9(PSCM), 10(PSCH), 7(PSCT),  8(PSCB), 0(nCELL), 11(nCEL2)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [SIZE-D] :  8(1CELL, 2CELL), 13(3CELL),  6(PSCM),  7(PSCH), 8(PSCT),  5(PSCB), 0(nCELL), 18(nCEL2)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [STIFF]  : AREA, ASy, ASz, Ixx, Iyy, Izz");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [SCHK]   : bAUTO_Z1, Z1, bAUTO_Z3, Z3");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [WT]     : bAUTO_TOR, TOR, bAUTO_SHR1, SHR1, bAUTO_SHR2, SHR2, bAUTO_SHR3, SHR3");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [CMPWEB] : EFD, LRF, A, B, H, T");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	File1.WriteString("\n");
	
	//str=_T("1, DBUSER    , 钢管截面          , CC, 0, 0, 0, 0, 0, 0, YES, P  , 2, 0.048, 0.0035, 0, 0, 0, 0, 0, 0, 0, 0\n");
	//File1.WriteString(str);
	SectionCount=1;
	for(i=1;i<=D_Deviation_Per[0];i++)
	{
		for(j=1;j<=tw_Deviation_Per[0];j++)
		{
			str.Format("    %d, DBUSER    , 钢管截面          , CC, 0, 0, 0, 0, 0, 0, YES, P  , 2, %.3f, %.4f, 0, 0, 0, 0, 0, 0, 0, 0\n\n",SectionCount++,D_Deviation[i],tw_Deviation[j]);
			File1.WriteString(str);
		}
	}
	
	
	
	
	
	
	
	//'荷载工况定义
	//*********** Lines = Lines & readFile("d:\参数化建模\支架文件\STLDCASE.mct")   *************
	str=_T("*STLDCASE    ; Static Load Cases");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; LCNAME, LCTYPE, DESC");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   杆系自重, D , ");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   浇筑和振捣混凝土, D , ");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   施工人员、材料、设备, D , ");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   预压荷载1, D , ");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   预压荷载2, D , ");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   预压荷载3, D , ");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   第一次浇筑, D , ");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   第二次浇筑, D , ");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   模板、支撑梁, D , ");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   防护设施、附加构件, D , ");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   风荷载, D , ");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	File1.WriteString("\n");
	
	
	
	
	
	//'一般边界条件定义
	//Lines = Lines & readFile("d:\参数化建模\支架文件\CONSTRAINT.mct")
	str=_T("*CONSTRAINT    ; Supports");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NODE_LIST, CONSt(Dx,Dy,Dz,Rx,Ry,Rz),GROUP");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	//Lines = Lines & "1to" & dc & ",001000,地基" & vbCrLf
	//str.Format("%s  %d  , %s", "1 to" ,dc ,"001000,地基");//田海涛
	dc=ZhuJuGeShu_X*PaiJuGeShu_Y;
	str.Format("%s  %d  , %s", "1 to" ,dc ,"111000,地基支撑");
	File1.WriteString(str);
	File1.WriteString("\n\n");
	
	str = _T("*FRAME-RLS    ; Beam End Release\n");
	File1.WriteString(str);
	str = _T("; ELEM_LIST, bVALUE, FLAG-i, Fxi, Fyi, Fzi, Mxi, Myi, Mzi        ; 1st line\n");
	File1.WriteString(str);
	str = _T(";                    FLAG-j, Fxj, Fyj, Fzj, Mxj, Myj, Mzj, GROUP ; 2nd line\n");
	File1.WriteString(str);
	HorizPoleNum=m2;//表示横向杆件总数
	float Mx=0.0,My=0.0;//田海涛修改
	((CEdit*)GetDlgItem(IDC_EDIT_HENG_MX))->GetWindowText(tempstr);
	Mx=atof(tempstr);
	((CEdit*)GetDlgItem(IDC_EDIT_HENG_MY))->GetWindowText(tempstr);
	My=atof(tempstr);
	for(i=1;i<=HorizPoleNum;i++)
	{
		str.Format("     %d,  YES, 000110, 0, 0, 0, %.2f, %.2f, 0\n             000110, 0, 0, 0, %.2f, %.2f, 0, 横杆-立杆\n",i,Mx,My,Mx,My);
		File1.WriteString(str);
	}
	//m3+1 to dy
	File1.WriteString("\n");
	((CEdit*)GetDlgItem(IDC_EDIT_TRIM_MX))->GetWindowText(tempstr);
	Mx=atof(tempstr);
	((CEdit*)GetDlgItem(IDC_EDIT_TRIM_MY))->GetWindowText(tempstr);
	My=atof(tempstr);
	for(i=m3+1;i<=dy;i++)
	{
		str.Format("     %d,  YES, 000110, 0, 0, 0, %.2f, %.2f, 0\n             000110, 0, 0, 0, %.2f, %.2f, 0, 剪刀撑-立杆\n",i,Mx,My,Mx,My);
		File1.WriteString(str);
	}

	File1.WriteString("\n");
	
	//'支架自重定义
	//Lines = Lines & readFile("d:\参数化建模\支架文件\SELFWEIGHT.mct")
	str=_T("*USE-STLD, 杆系自重");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	
	str=_T("; *SELFWEIGHT, X, Y, Z, GROUP");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("*SELFWEIGHT, 0, 0, -1, 支架自重");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	
	str=_T("; End of data for load case [杆系自重] -------------------------");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	
	
	
	//'浇筑及振捣混凝土荷载定义
	//Lines = Lines & readFile("d:\参数化建模\支架文件\USE-STLD-q5.mct")
	str=_T("*USE-STLD, 浇筑和振捣混凝土");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*CONLOAD    ; Nodal Loads");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NODE_LIST, FX, FY, FZ, MX, MY, MZ, GROUP");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	//	a=ZhuJuGeShu_X-1;
	//	b=PaiJuGeShu_Y-1;
	//	c=BuJuGeShu_Z-3;
	
	//qq5 = -JiaoZhuZhenDao/((b - 1) * (a + 1));
	qq5 = -JiaoZhuZhenDao/((PaiJuGeShu_Y - 2) *ZhuJuGeShu_X);
	
	for (int yyy = 0 ;yyy<ZhuJuGeShu_X;yyy++)
	{
		for ( u = (BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X+2+yyy*PaiJuGeShu_Y;u<=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X+(PaiJuGeShu_Y-1)+yyy*PaiJuGeShu_Y;u++)
		{
			//Lines = Lines & u & ",0,0," & qq5 & ",0,0,0,浇筑及振捣" & vbCrLf
			str.Format("%d , %d , %d , %f , %d , %d , %d , %s",u,0,0,qq5,0,0,0,"浇筑及振捣");
			File1.WriteString(str);
			File1.WriteString("\n");
		}
	}
	
	//'施工人员材料机械荷载定义
	//Lines = Lines & readFile("d:\参数化建模\支架文件\USE-STLD-q4.mct")
	str=_T("; End of data for load case [浇筑和振捣混凝土] -------------------------");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*USE-STLD, 施工人员、材料、设备");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*CONLOAD    ; Nodal Loads");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NODE_LIST, FX, FY, FZ, MX, MY, MZ, GROUP");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	//	a=ZhuJuGeShu_X-1;
	//	b=PaiJuGeShu_Y-1;
	//	c=BuJuGeShu_Z-3;
	
	
	qq4 = -RenChaiJi/((PaiJuGeShu_Y-2)*ZhuJuGeShu_X);
	for (yyy = 0 ;yyy<ZhuJuGeShu_X;yyy++)
	{
		for (u=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X+2+yyy*PaiJuGeShu_Y;u<=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X+(PaiJuGeShu_Y-1)+yyy*PaiJuGeShu_Y;u++)
		{
			//Lines = Lines & u & ",0,0," & qq4 & ",0,0,0,施工人员机械" & vbCrLf
			str.Format("%d , %d , %d , %f , %d , %d , %d , %s",u,0,0,qq4,0,0,0,"施工人员机械");
			File1.WriteString(str);
			File1.WriteString("\n");
		}
	}
	//'第一次预压荷载定义
	//Lines = Lines & readFile("d:\参数化建模\支架文件\USE-STLD-y1.mct")
	str=_T("; End of data for load case [施工人员、材料、设备] -------------------------");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*USE-STLD, 预压荷载1");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*CONLOAD    ; Nodal Loads");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NODE_LIST, FX, FY, FZ, MX, MY, MZ, GROUP");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	
	//	a=ZhuJuGeShu_X-1;
	//	b=PaiJuGeShu_Y-1;
	//	c=BuJuGeShu_Z-3;
	
	qy1 = -DiYiCiYuYa * qy / ((PaiJuGeShu_Y-6) * ZhuJuGeShu_X);
	for (yyy = 0 ;yyy<ZhuJuGeShu_X;yyy++)
	{
		for (u=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X+4+yyy*PaiJuGeShu_Y;u<=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X-2+(PaiJuGeShu_Y-1)+yyy*PaiJuGeShu_Y;u++)
		{//u = (c + 2) * dc + 4 + yyy * (b + 1) ;u<= (c + 2) * dc + b - 2 + yyy * (b + 1);u++
			//Lines = Lines & u & ",0,0," & qy1 & ",0,0,0,预压第一次" & vbCrLf
			str.Format("%d , %d , %d , %f , %d , %d , %d , %s",u,0,0,qy1,0,0,0,"预压第一次");
			File1.WriteString(str);
			File1.WriteString("\n");
		}
	}
	//'第二次预压荷载定义
	//Lines = Lines & readFile("d:\参数化建模\支架文件\USE-STLD-y2.mct")
	str=_T("; End of data for load case [预压荷载1] -------------------------");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*USE-STLD, 预压荷载2");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*CONLOAD    ; Nodal Loads");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NODE_LIST, FX, FY, FZ, MX, MY, MZ, GROUP");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	
	qy2 = -DiErCiYuYa * qy / ((PaiJuGeShu_Y-6) * ZhuJuGeShu_X);
	for (yyy = 0 ;yyy<ZhuJuGeShu_X;yyy++)
	{
		for (u=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X+4+yyy*PaiJuGeShu_Y;u<=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X-2+(PaiJuGeShu_Y-1)+yyy*PaiJuGeShu_Y;u++)
		{
			//Lines = Lines & u & ",0,0," & qy2 & ",0,0,0,预压第二次" & vbCrLf
			str.Format("%d , %d , %d , %f , %d , %d , %d , %s",u,0,0,qy2,0,0,0,"预压第二次");
			File1.WriteString(str);
			File1.WriteString("\n");
		}
	}
	//'第三次预压荷载定义
	//Lines = Lines & readFile("d:\参数化建模\支架文件\USE-STLD-y3.mct")
	str=_T("; End of data for load case [预压荷载2] -------------------------");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*USE-STLD, 预压荷载3");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*CONLOAD    ; Nodal Loads");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NODE_LIST, FX, FY, FZ, MX, MY, MZ, GROUP");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	qy3 = -DiSanCiYuYa * qy / ((PaiJuGeShu_Y-6) * ZhuJuGeShu_X);
	for (yyy = 0 ;yyy<ZhuJuGeShu_X;yyy++)
	{
		for (u=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X+4+yyy*PaiJuGeShu_Y;u<=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X-2+(PaiJuGeShu_Y-1)+yyy*PaiJuGeShu_Y;u++)
		{
			//Lines = Lines & u & ",0,0," & qy3 & ",0,0,0,预压第三次" & vbCrLf
			str.Format("%d , %d , %d , %f , %d , %d , %d , %s",u,0,0,qy3,0,0,0,"预压第三次");
			File1.WriteString(str);
			File1.WriteString("\n");
		}
	}
	//'第一次浇筑荷载定义
	//Lines = Lines & readFile("d:\参数化建模\支架文件\USE-STLD-j1.mct")
	str=_T("; End of data for load case [预压荷载3] -------------------------");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*USE-STLD, 第一次浇筑");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*CONLOAD    ; Nodal Loads");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NODE_LIST, FX, FY, FZ, MX, MY, MZ, GROUP");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	
	qj1 = -DiYiCiJiaoZhu * GangJinHunNingTu / ((PaiJuGeShu_Y-6) * ZhuJuGeShu_X);
	for (yyy = 0 ;yyy<ZhuJuGeShu_X;yyy++)
	{
		for (u=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X+4+yyy*PaiJuGeShu_Y;u<=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X-2+(PaiJuGeShu_Y-1)+yyy*PaiJuGeShu_Y;u++)
		{
			//Lines = Lines & u & ",0,0," & qj1 & ",0,0,0,浇筑第一次" & vbCrLf
			str.Format("%d , %d , %d , %f , %d , %d , %d , %s",u,0,0,qj1,0,0,0,"浇筑第一次");
			File1.WriteString(str);
			File1.WriteString("\n");
		}
	}
	
	//'第二次浇筑荷载定义
	//Lines = Lines & readFile("d:\参数化建模\支架文件\USE-STLD-j2.mct")
	str=_T("; End of data for load case [第一次浇筑] -------------------------");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*USE-STLD, 第二次浇筑");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*CONLOAD    ; Nodal Loads");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NODE_LIST, FX, FY, FZ, MX, MY, MZ, GROUP");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	
	qj2 = -DiErCiJiaoZhu * GangJinHunNingTu / ((PaiJuGeShu_Y-6) * ZhuJuGeShu_X);
	for (yyy = 0 ;yyy<ZhuJuGeShu_X;yyy++)
	{
		for (u=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X+4+yyy*PaiJuGeShu_Y;u<=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X-2+(PaiJuGeShu_Y-1)+yyy*PaiJuGeShu_Y;u++)
		{
			//Lines = Lines & u & ",0,0," & qj2 & ",0,0,0,浇筑第二次" & vbCrLf
			str.Format("%d , %d , %d , %f , %d , %d , %d , %s",u,0,0,qj2,0,0,0,"浇筑第二次");
			File1.WriteString(str);
			File1.WriteString("\n");
		}
	}
	
	//Print #2, Lines
	//Lines = ""
	
	//'模板支撑梁荷载定义
	//Lines = Lines & readFile("d:\参数化建模\支架文件\USE-STLD-q2.mct")
	str=_T("; End of data for load case [第二次浇筑] -------------------------");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*USE-STLD, 模板、支撑梁");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*CONLOAD    ; Nodal Loads");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NODE_LIST, FX, FY, FZ, MX, MY, MZ, GROUP");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	
	
	qq2 = -MoBanFangLeng / ((PaiJuGeShu_Y-2) * ZhuJuGeShu_X);
	for (yyy = 0 ;yyy<ZhuJuGeShu_X;yyy++)
	{
		for (u=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X+2+yyy*PaiJuGeShu_Y;u<=(BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X+(PaiJuGeShu_Y-1)+yyy*PaiJuGeShu_Y;u++)
		{
			//Lines = Lines & u & ",0,0," & qq2 & ",0,0,0,模板方楞等" & vbCrLf 
			str.Format("%d , %d , %d , %f , %d , %d , %d , %s",u,0,0,qq2,0,0,0,"模板方楞等");
			File1.WriteString(str);
			File1.WriteString("\n");
		}
	}
	//'附加构件等荷载定义
	//Lines = Lines & readFile("d:\参数化建模\支架文件\USE-STLD-q3.mct")
	str=_T("; End of data for load case [模板、支撑梁] -------------------------");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*USE-STLD, 防护设施、附加构件");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*CONLOAD    ; Nodal Loads");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NODE_LIST, FX, FY, FZ, MX, MY, MZ, GROUP");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	
	qq3 = -FuJiaGouJian / (2*ZhuJuGeShu_X);
	for (yyy = 0 ;yyy<ZhuJuGeShu_X;yyy++)
	{
		u = (BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X + 1 + yyy * PaiJuGeShu_Y;
		//Lines = Lines & u & ",0,0," & qq3 & ",0,0,0,附加构件" & vbCrLf
		str.Format("%d , %d , %d , %f , %d , %d , %d , %s",u,0,0,qq3,0,0,0,"附加构件");
		File1.WriteString(str);
		File1.WriteString("\n");
	}
	
	for (yyy = 0 ;yyy<ZhuJuGeShu_X;yyy++)
	{
		u = (BuJuGeShu_Z-1)*PaiJuGeShu_Y*ZhuJuGeShu_X + PaiJuGeShu_Y + yyy * PaiJuGeShu_Y;
		//Lines = Lines & u & ",0,0," & qq3 & ",0,0,0,附加构件" & vbCrLf
		str.Format("%d , %d , %d , %f , %d , %d , %d , %s",u,0,0,qq3,0,0,0,"附加构件");
		File1.WriteString(str);
		File1.WriteString("\n");
	}
	
	//'风荷载定义
	//Lines = Lines & readFile("d:\参数化建模\支架文件\USE-STLD-f.mct")
	str=_T("; End of data for load case [防护设施、附加构件] -------------------------");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*USE-STLD, 风荷载");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*CONLOAD    ; Nodal Loads");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NODE_LIST, FX, FY, FZ, MX, MY, MZ, GROUP");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	
	//	a=ZhuJuGeShu_X-1;
	//	b=PaiJuGeShu_Y-1;
	//	c=BuJuGeShu_Z-3;
	
	
	qq6 = FengHeZai / (3 * ZhuJuGeShu_X);
	for (int xxx = 0 ;xxx<= 2;xxx++)
	{
		for (yyy = 0 ;yyy<ZhuJuGeShu_X;yyy++)
		{
			u = (BuJuGeShu_Z-3)*ZhuJuGeShu_X*PaiJuGeShu_Y + 1 + yyy * PaiJuGeShu_Y + ZhuJuGeShu_X*PaiJuGeShu_Y * xxx;
			//Lines = Lines & u & ",0," & qq6 & ",0,0,0,0,风荷载" & vbCrLf 
			str.Format("%d , %d , %f , %d , %d , %d , %d , %s",u,0,qq6,0,0,0,0,"风荷载");
			File1.WriteString(str);
			File1.WriteString("\n");
		}
	}
	//'荷载组合
	//Lines = Lines & readFile("d:\参数化建模\支架文件\LOADCOMB.mct")
	str=_T("; End of data for load case [风荷载] -------------------------");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	File1.WriteString("\n");
	str=_T("*LOADCOMB    ; Combinations");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; NAME=NAME, KIND, ACTIVE, bES, iTYPE, DESC, iSERV-TYPE, nLCOMTYPE   ; line 1");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";      ANAL1, LCNAME1, FACT1, ...                                    ; from line 2");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   NAME=第一次预压, GEN, ACTIVE, 0, 0, , 0, 0");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("        ST, 杆系自重, 1, ST, 预压荷载1, 1, ST, 模板、支撑梁, 1");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("        ST, 防护设施、附加构件, 1, ST, 风荷载, 1");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   NAME=第二次预压, GEN, ACTIVE, 0, 0, , 0, 0");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("        ST, 杆系自重, 1, ST, 预压荷载1, 1, ST, 预压荷载2, 1");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("        ST, 模板、支撑梁, 1, ST, 防护设施、附加构件, 1, ST, 风荷载, 1");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   NAME=第三次预压, GEN, ACTIVE, 0, 0, , 0, 0");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("        ST, 杆系自重, 1, ST, 预压荷载1, 1, ST, 预压荷载2, 1, ST, 预压荷载3, 1");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("        ST, 模板、支撑梁, 1, ST, 防护设施、附加构件, 1, ST, 风荷载, 1");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   NAME=第一次浇筑, GEN, ACTIVE, 0, 0, , 0, 0");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("        ST, 杆系自重, 1, ST, 第一次浇筑, 1, ST, 浇筑和振捣混凝土, 1");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("        ST, 施工人员、材料、设备, 1, ST, 模板、支撑梁, 1");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("        ST, 防护设施、附加构件, 1, ST, 风荷载, 1");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("   NAME=第二次浇筑, GEN, ACTIVE, 0, 0, , 0, 0");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("        ST, 杆系自重, 1, ST, 第一次浇筑, 1, ST, 第二次浇筑, 1");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("        ST, 浇筑和振捣混凝土, 1, ST, 施工人员、材料、设备, 1");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("        ST, 模板、支撑梁, 1, ST, 防护设施、附加构件, 1, ST, 风荷载, 1");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	
	
	str=_T("*LC-COLOR    ; Diagram Color for Load Case");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; ANAL, LCNAME, iR1(ALL), iG1(ALL), iB1(ALL), iR2(MIN), iG2(MIN), iB2(MIN), iR3(MAX), iG2(MAX), iB2(MAX)");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" ST, 预压荷载1, 0, 192, 192, 0, 128, 57, 255, 255, 87");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" ST, 预压荷载2, 163, 255, 160, 210, 210, 210, 0, 128, 192");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" ST, 杆系自重, 160, 192, 255, 148, 87, 255, 0, 192, 128");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" CB, 第一次预压, 192, 192, 0, 0, 192, 192, 192, 192, 192");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" CB, 第二次预压, 78, 0, 255, 0, 157, 192, 160, 255, 255");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" CB, 第三次预压, 192, 0, 128, 192, 0, 128, 93, 255, 87");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" ST, 预压荷载3, 93, 255, 87, 0, 128, 192, 0, 192, 192");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" ST, 模板、支撑梁, 255, 160, 255, 146, 0, 255, 163, 255, 160");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" ST, 浇筑和振捣混凝土, 0, 192, 128, 0, 128, 192, 0, 192, 128");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" ST, 施工人员、材料、设备, 148, 87, 255, 192, 128, 0, 192, 128, 0");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" CB, 第一次浇筑, 255, 160, 255, 210, 210, 210, 255, 0, 128");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" ST, 第一次浇筑, 163, 160, 255, 93, 255, 87, 192, 192, 192");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" ST, 第二次浇筑, 192, 0, 192, 0, 128, 255, 212, 160, 255");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" ST, 防护设施、附加构件, 0, 128, 57, 0, 192, 128, 0, 192, 128");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" CB, 第二次浇筑, 192, 72, 0, 148, 87, 255, 192, 0, 192");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(" ST, 风荷载, 255, 128, 0, 192, 0, 192, 85, 0, 192");
	File1.WriteString(str);
	File1.WriteString("\n");
	File1.WriteString("\n");
	
	
	str=_T("*DGN-MATL    ; Modify Steel(Concrete) Material");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iMAT, TYPE, MNAME, [DATA1]                                    ; STEEL");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iMAT, TYPE, MNAME, [DATA2], [R-DATA], FCI, bSERV, SHORT, LONG ; CONC");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iMAT, TYPE, MNAME, [DATA3], [DATA2], [R-DATA]                 ; SRC");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; iMAT, TYPE, MNAME, [DATA5]                                    ; STEEL(None) & KSCE-ASD05");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DATA1] : 1, DB, CODE, NAME or 2, ELAST, POISN, FU, FY1, FY2, FY3, FY4");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";           FY5, FY6, AFT, AFT2, AFT3, FY, AFV, AFV2, AFV3");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DATA2] : 1, DB, CODE, NAME or 2, FC");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DATA3] : 1, DB, CODE, NAME or 2, ELAST, FU, FY1, FY2, FY3, FY4");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";              FY5, FY6, AFT, AFT2, AFT3, FY, AFV, AFV2, AFV3");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DATA4] : 1, DB, CODE, NAME or 2, FC");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [DATA5] : 3, ELAST, POISN, AL1, AL2, AL3, AL4, AL5, AL6, AL7, AL8, AL9, AL10");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T(";              MIN1, MIN2, MIN3");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("; [R-DATA]: RBCODE, RBMAIN, RBSUB, FY(R), FYS");
	File1.WriteString(str);
	File1.WriteString("\n");
	str=_T("    1, STEEL, Q235              , 1, GB03(S)    ,            ,Q235          ");
	File1.WriteString(str);
	File1.WriteString("\n");
	
	//Print #2, Lines
	//Lines = ""
	
	//End Sub
	
	
	num=1;
	
	File1.Close();
	AfxMessageBox("success");
	return;
	
	//AfxMessageBox("导出成功！");
	
}

void CSet::init()
{
	int q;
	for(q=0;q<10;q++)
	{
		PaiJuDealSave[q]=0;
	}
	for(q=0;q<10;q++)
	{
		PaiJuCountSave[q]=0;
	}
	for(q=0;q<10;q++)
	{
		PaiJuDataSave[q]=0;
	}
	
	
}

BOOL CSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitVal();
	//m_Control_BuJu_Z.enablein
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//m_Control_ZhuJu_X
	//m_button_Output.SetSkin(IDB_PUTONG_WAICENG,IDB_PUTONG_WAICENG,IDB_PUTONG_WAICENG,IDB_MASK_WAICENG,0,IDB_MASK_WAICENG,0,0,0);
	//m_button_Clear.SetSkin(IDB_PUTONG_WAICENG,IDB_PUTONG_WAICENG,IDB_PUTONG_WAICENG,IDB_MASK_WAICENG,0,IDB_MASK_WAICENG,0,0,0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSet::OnClear() 
{
    BuJuGeShu_Z=0;
	ZhuJuGeShu_X=0;
	PaiJuGeShu_Y=0;
	init();
	
   	m_ZhuJuX.Empty();
	m_PaiJu_Y.Empty();
	m_BujuZ.Empty();
	m_DingCengXuanBi.Empty();
	m_SaoDiGanGaoDu.Empty();
	m_GangJinHunNingTu.Empty();
	m_MoBanFangLeng.Empty();
	m_FuJiaGouJian.Empty();
	m_RenChaiJi.Empty();
	m_JiaoZhuZhenDao.Empty();
	m_FengHeZai.Empty();
    m_DiYiCiYuYa.Empty();
	m_DiErCiYuYa.Empty();
	m_DiSanCiYuYa.Empty();
	m_DiYiCiJiaoZhu.Empty();
	m_DiErCiJiaoZhu.Empty();
	
	
	
	m_Control_ZhuJu_X.SetCurSel(-1);
	m_Control_BuJu_Z.SetCurSel(-1);
	Sort=0;
	PaiJuCount=0;
	ZhuJuGeShu_X=0;
	BuJuGeShu_Z=0;
	PaiJuGeShu_Y=0;
	NodeZongShu=0;
	Length_X=0;
	Length_Y=0;
	Length_Z=0;
	
	
	ini_GangJinHunNingTu=0.0;
	ini_MoBanFangLeng=0.0;
	ini_FuJiaGouJian=0.0;
	ini_RenChaiJi=0.0;
	ini_JiaoZhuZhenDao=0.0;
	ini_FengHeZai=0.0;
	
	
	//************************修改VB程序设定的变量************************//
	a=0;
	c=0;;
	b=0;
	dy=1;               //单元编号
	dc=0      ;            //单层节点数
	qd=  0      ;           //单元前节点
	zd=  0      ;           //单元后节点
	
	//************************修改VB程序斜向单元设定的变量****************************//
	
	J1=1;
	j2=1;
	i1=1;
	i2=1;
	is1=0;
	s1=1;
	s2=1;
	k1=0;
	k2=0;
	k3=0;
	k4=0;
	
	//************************循环变量设定*************************//
	xx=0;     //专业循环变量
	yy=0;     //专业循环变量
	zz=0;     //专业循环变量
	rr=0; 
	tt=0;
	kk=0;
	yj1=0;
	jy1=0;
	hh=0;
	ee=0;
	ff=0;
	cc=0;
	xi1=0;
	
	m1=0;
	m2=0;
	m3=0;
	u=0;
	qy=0;
	
	
	
	UpdateData(FALSE);
	
}

void CSet::OnButtonInputexcel() 
{
	CFileDialog file(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("EXCEL文件t(*.xls;*.xlsx)|*.xls;*.xlsx||"),AfxGetMainWnd());
	if(file.DoModal()!=IDOK)
	{   
		AfxMessageBox("选择窗口未打开");
		return;
	}
	CString strFile=file.GetPathName();
	m_Path.SetWindowText(strFile);
	int i=0,j=0,k=0;
	CString tempstr = "";
	vector < CString > CellsVal(20);
	CellsVal.resize(20);
	//CellsVal[0]="123";
	//memset(CellsVal,0,sizeof(CellsVal)/sizeof(CellsVal[0][0]));
	//GetRangeVal(&strFile,NULL,"J3","J20",&CellsVal);
	for(i=0;i<20;i++)
	{
		tempstr = tempstr+CellsVal[i]+"||";
	}
	//AfxMessageBox(tempstr);
	//return;
	_ApplicationE app;
	_Workbook book;
	_Worksheet sheet;
	Workbooks books;
	Worksheets sheets;
	Range range;
	LPDISPATCH lpDisp;
	COleVariant vResult;
	COleVariant covTrue((short)TRUE);
	COleVariant covFalse((short)FALSE);
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);	
	//创建Excel服务器(启动Excel)
	if(!app.CreateDispatch("Excel.Application"))
	{
		AfxMessageBox("无法启动Excel服务器!");
		return;
	}
	app.SetVisible(FALSE);          //使Excel不可见	
	books.AttachDispatch(app.GetWorkbooks());
	//CString strFile = "E:\\Desktop\\梁单元应力0611.xlsx";
	lpDisp = books.Open(strFile, 
		covOptional, covFalse, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional);
	
	//得到Workbook
	book.AttachDispatch(lpDisp);
	//CString bookname = book.GetName();
	
	//得到Worksheets
	//CString tempstr = "梁单元应力";
	//sheets.Select(COleVariant(tempstr));
	sheets.AttachDispatch(book.GetWorksheets());//
	//CString sheetname = "";
	for(i=0;;i++)
	{
		lpDisp=sheets.GetItem(COleVariant(short(i+1)));//book.GetActiveSheet();
		sheet.AttachDispatch(lpDisp);
		if(sheet.GetName()=="Sheet1")
			break;	
	}
	//sheetname.Format("sheet num = %d",i);
	//AfxMessageBox(sheetname);
	
	int SheetNum = i;
	int m_ValCount[8];
	memset(m_ValCount,0,sizeof(m_ValCount)/sizeof(m_ValCount[0])*sizeof(int));
	vector < vector < CString > > m_GetCVal;
	vector < vector < CString > > m_GetDVal;
	vector < vector < CString > > m_GetHVal;
	vector < vector < CString > > m_GetJVal;
	/*	vector < CString > m_GetCVal0;
	vector < CString > m_GetDVal0;
	vector < CString > m_GetHVal0;
	vector < CString > GetIVal0;
	vector < CString > m_GetJVal0;*/
	GetCVal.clear();
	GetDVal.clear();
	GetHVal.clear();
	GetJVal.clear();
	GetCVal.resize(20);
	GetDVal.resize(20);
	GetHVal.resize(20);
	GetJVal.resize(20);
	m_GetCVal.clear();
	m_GetDVal.clear();
	m_GetHVal.clear();
	m_GetJVal.clear();
	m_GetCVal.resize(SheetNum);
	m_GetDVal.resize(SheetNum);
	m_GetHVal.resize(SheetNum);
	m_GetJVal.resize(SheetNum);
	for(i=0;i<SheetNum;i++)
	{
		m_GetCVal[i].resize(20);
	}
	for(i=0;i<SheetNum;i++)
	{
		m_GetDVal[i].resize(20);
	}
	for(i=0;i<SheetNum;i++)
	{
		m_GetHVal[i].resize(20);
	}
	for(i=0;i<SheetNum;i++)
	{
		m_GetJVal[i].resize(20);
	}
	Range usedRange;
	long RowNum=0;
	long ColNum=0;
	CString EndCellCode = "";
	VARIANT key1; // these lines set up first arg (key1) to sort
	Range iRange;
	CString tempSCell = "";
	CString tempECell = "";
	VARIANT varRead;
	COleSafeArray olesaRead;
	CString minJcodeCVal = "";
	CString maxJcodeCVal = "";
	int SRow = 0;
	float tempJVal=0.0;
	VARIANT TempvarRead;
	//得到当前活跃sheet
	//如果有单元格正处于编辑状态中，此操作不能返回，会一直等待
	SetProcess(0);
	for(i=0;i<SheetNum;i++)
	{
		lpDisp=sheets.GetItem(COleVariant(short(i+1)));//book.GetActiveSheet();
		sheet.AttachDispatch(lpDisp);
		//CString sheetname = sheet.GetName();
		
		//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
		usedRange.AttachDispatch(sheet.GetUsedRange());
		range.AttachDispatch(usedRange.GetRows());
		RowNum=range.GetCount();                   //已经使用的行数
		range.ReleaseDispatch();
		range.AttachDispatch(usedRange.GetColumns());	
		ColNum=range.GetCount()-1;               //获取有效列数目，获取结果总比实际多1
		EndCellCode.Format("%s%d",TranslateColName(ColNum),RowNum);
		
		V_VT(&key1) = VT_DISPATCH;                           // 排序时，关键字的vt设置为VT_DISPATCH
		
		V_DISPATCH(&key1) = sheet.GetRange(COleVariant("J1"),COleVariant("J1"));   // 设置按哪个关键字进行排序 选择单元意为：按此列（或行）为主关键字进行排序	
		//range=sheet.GetRange(COleVariant(L"A1"),COleVariant(CellName));
		if(i==0)// 选择对哪些区域内的单元格进行排序
		{
			iRange = sheet.GetRange(COleVariant("C3"),COleVariant(EndCellCode));
			tempSCell.Format("C3");
			tempECell.Format("C4");
		}
		else
		{
			iRange = sheet.GetRange(COleVariant("C2"),COleVariant(EndCellCode));
			tempSCell.Format("C2");
			tempECell.Format("C3");
		}
		iRange.Sort(key1, xlAscending, vOpt, vOpt, xlAscending, vOpt,xlAscending,
			xlNoHeader,vOpt,xlIgnoreCase,xlTopToBottom,xlPinYin,0,0,0); //设置排序 无标题时一定要设置为xlNoHeader  否则不起作用
		
		////////////////////////////////////////////////////////////////////////////
		lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
		iRange.AttachDispatch(lpDisp);
		olesaRead.Clear();
		varRead = iRange.GetValue2();
		olesaRead.Attach(varRead);
		GetValFromArry(&olesaRead,&CellsVal);
		olesaRead.Detach();
		minJcodeCVal = CellsVal[0];
		//AfxMessageBox(minJcodeCVal);
		
		
		tempSCell.Format("C%d",RowNum);
		tempECell.Format("C%d",RowNum+1);
		lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
		iRange.AttachDispatch(lpDisp);
		olesaRead.Clear();
		varRead = iRange.GetValue2();
		olesaRead.Attach(varRead);
		GetValFromArry(&olesaRead,&CellsVal);
		maxJcodeCVal = CellsVal[0];
		//AfxMessageBox(maxJcodeCVal);
		V_DISPATCH(&key1) = sheet.GetRange(COleVariant("C1"),COleVariant("C1"));   // 设置按哪个关键字进行排序 选择单元意为：按此列（或行）为主关键字进行排序	
		if(i==0)
			lpDisp = sheet.GetRange(COleVariant("C3"), COleVariant(EndCellCode));
		else
			lpDisp = sheet.GetRange(COleVariant("C2"), COleVariant(EndCellCode));
		iRange.AttachDispatch(lpDisp);
		iRange.Sort(key1, xlAscending, vOpt, vOpt, xlAscending, vOpt,xlAscending,
			xlNoHeader,vOpt,xlIgnoreCase,xlTopToBottom,xlPinYin,0,0,0); //设置排序 无标题时一定要设置为xlNoHeader  否则不起作用
		
		m_ValCount[i]=0;
		if(i==0)
			SRow=3;
		else
			SRow=2;
		for(;SRow<=RowNum;SRow++)
		{
			SetProcess(50);
			tempSCell.Format("C%d",SRow);
			tempECell.Format("C%d",SRow+1);
			lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
			iRange.AttachDispatch(lpDisp);
			VARIANT varRead = iRange.GetValue2();
			COleSafeArray olesaRead(varRead);
			GetValFromArry(&olesaRead,&CellsVal);
			olesaRead.Detach();
			if((minJcodeCVal == CellsVal[0])||(maxJcodeCVal == CellsVal[0]))
			{
				COleSafeArray olesaRead;
				m_GetCVal[i][m_ValCount[i]].Format("%d",_ttoi(CellsVal[0]));
				tempSCell.Format("D%d",SRow);
				tempECell.Format("D%d",SRow+1);
				lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
				iRange.AttachDispatch(lpDisp);
				TempvarRead = iRange.GetValue2();
				olesaRead.Attach(TempvarRead);
				GetValFromArry(&olesaRead,&CellsVal);
				olesaRead.Detach();
				m_GetDVal[i][m_ValCount[i]]=CellsVal[0];
				
				tempSCell.Format("H%d",SRow);
				tempECell.Format("H%d",SRow+1);
				lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
				iRange.AttachDispatch(lpDisp);
				TempvarRead = iRange.GetValue2();
				olesaRead.Attach(TempvarRead);
				GetValFromArry(&olesaRead,&CellsVal);
				olesaRead.Detach();
				m_GetHVal[i][m_ValCount[i]]=CellsVal[0];
				
				tempSCell.Format("J%d",SRow);
				tempECell.Format("J%d",SRow+1);
				lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
				iRange.AttachDispatch(lpDisp);
				TempvarRead = iRange.GetValue2();
				olesaRead.Attach(TempvarRead);
				GetValFromArry(&olesaRead,&CellsVal);
				olesaRead.Detach();
				tempJVal = atof(CellsVal[0]);
				m_GetJVal[i][m_ValCount[i]++].Format("%.4f",tempJVal/1000);
				
			}
			SetProcess(100);
		}
	}
	
	
	//book.Save();
    book.Close(covFalse,COleVariant(strFile),covOptional);
    books.Close();      
    app.Quit();
	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app.ReleaseDispatch();
	tempstr = "";
	//float FinalVal[8][20];
	CString FinalMax = m_GetJVal[0][0];
	CString FinalMin = m_GetJVal[0][0];
	for(j=0;j<SheetNum;j++)
	{
		for(i=0;i<m_ValCount[j];i++)
		{
			//FinalVal[j][i]=atof(m_GetJVal[j][i]);
			FinalMax = atof(FinalMax)>atof(m_GetJVal[j][i])?FinalMax:m_GetJVal[j][i];
			FinalMin = atof(FinalMin)<atof(m_GetJVal[j][i])?FinalMin:m_GetJVal[j][i];
			tempstr = tempstr+ m_GetCVal[j][i]+","+m_GetDVal[j][i]+","+m_GetHVal[j][i]+","+m_GetJVal[j][i]+"\n";
		}
		tempstr = tempstr+"\n";
	}
	//AfxMessageBox(tempstr);
	//tempstr.Format("max = %s,min = %s",FinalMax,FinalMin);
	//AfxMessageBox(tempstr);
	int l=0;
	ValCount = 0;
	for(j=0;j<SheetNum;j++)
	{
		for(i=0;i<m_ValCount[j];i++)
		{
			if(m_GetJVal[j][i]==FinalMax||m_GetJVal[j][i]==FinalMin)
			{
				//m_GetCVal[j][i]
				for(k=0;k<SheetNum;k++)
				{
					for(l=0;l<m_ValCount[k];l++)
					{
						if(m_GetCVal[j][i]==m_GetCVal[k][l])
						{
							GetCVal[ValCount] = m_GetCVal[k][l];
							GetDVal[ValCount] = m_GetDVal[k][l];
							GetHVal[ValCount] = m_GetHVal[k][l];
							GetJVal[ValCount++] = m_GetJVal[k][l];
						}
					}
				}
			}
		}
	}
	for(i=0;i<ValCount;i++)
	{
		//FinalVal[j][i]=atof(m_GetJVal[j][i]);
		//FinalMax = atof(FinalMax)>atof(m_GetJVal[j][i])?FinalMax:m_GetJVal[j][i];
		//FinalMin = atof(FinalMin)<atof(m_GetJVal[j][i])?FinalMin:m_GetJVal[j][i];
		//tempstr = tempstr+ GetCVal[i]+","+GetDVal[i]+","+GetHVal[i]+","+GetJVal[i]+"\n";
	}
	//AfxMessageBox(tempstr);
	AfxMessageBox("读取应力数据成功！");
	return;
}



void CSet::OnRadioG1() 
{
	GradeID=5;	
	((CButton*)GetDlgItem(IDC_RADIO_G1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_G2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_G3))->SetCheck(FALSE);
}

void CSet::OnRadioG2() 
{
	GradeID=3;	
	((CButton*)GetDlgItem(IDC_RADIO_G2))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_G1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_G3))->SetCheck(FALSE);
}

void CSet::OnRadioG3() 
{
	GradeID=2;	
	((CButton*)GetDlgItem(IDC_RADIO_G3))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_G2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_G1))->SetCheck(FALSE);
}

CString CSet::TranslateColName(long ColNum)
{
	char ColumnCode[3]={0};
	ColumnCode[1]=ColNum%26+'A'-1;
	if(ColNum>26)
	{
		ColumnCode[0]=ColNum/26+'A'-1;
	}
	else
	{
		ColumnCode[0]=ColumnCode[1];
		ColumnCode[1]=0;
	}
	CString result("");
	result.Format("%s",ColumnCode);
	return result;
	
}

void CSet::GetValFromArry(COleSafeArray* olesaRead, vector < CString >* Valbuf)
{
	
    VARIANT varItem;
    CString strItem;
    long index[2] = {0, 0};
    long lFirstLBound = 0;
    long lFirstUBound = 0;
    long lSecondLBound = 0;
    long lSecondUBound = 0;
	int i=0,j=0,k=0;
    lFirstLBound = 0;
    lFirstUBound = 0;
    lSecondLBound = 0;
    lSecondUBound = 0;
    olesaRead->GetLBound(1, &lFirstLBound);
    olesaRead->GetUBound(1, &lFirstUBound);
    olesaRead->GetLBound(2, &lSecondLBound);
    olesaRead->GetUBound(2, &lSecondUBound);
    memset(index, 0, 2 * sizeof(long));
	CString tempstr = "";
    for (i = lFirstLBound; i <= lFirstUBound; i++)
    {
        index[0] = i;
        for (j = lSecondLBound; j <= lSecondUBound; j++)
        {
            index[1] = j;
            olesaRead->GetElement(index, &varItem);
			
            switch (varItem.vt)
            {
            case VT_R8:
                {
                    strItem.Format(_T("%.4f"), varItem.dblVal);
					break;
                }
            case VT_R4:
                {
                    strItem.Format(_T("%.2f"), varItem.fltVal);
					break;
                }
            case VT_BSTR:
                {
                    strItem = varItem.bstrVal;
					break;
                }
            case VT_I2:
                {
                    strItem.Format(_T("%d"), (int)varItem.iVal);
					break;
                }
            case VT_I4:
                {
                    strItem.Format(_T("%ld"), (int)varItem.lVal);
					break;
                }
            default:
                {
					strItem="0";
                }
            }
			(*Valbuf)[k++] = strItem;
        }
    }
}
void CSet::OnButtonOutputword() 
{	//需先导入xls文件读取应力极值处单元号
	int m=PaiJuGeShu_Y-1;//y向单元数
	int n=ZhuJuGeShu_X-1;//x向单元数
	int r=BuJuGeShu_Z-1;//z向单元数
	if(PaiJuGeShu_Y==0||ZhuJuGeShu_X==0||BuJuGeShu_Z==0)
	{
		AfxMessageBox("支架杆件xyz至少有一个方向为0,结构不足以生成监测方案\n请确认执行过[生成MCT]");
		return;
	}
	if(ValCount==0)
	{
		if(ValCount0==0)
		{
			if(IDNO==AfxMessageBox("未导入应力及位移Excel文件,是否继续生成监测方案?",MB_YESNO))
			{
				return;
			}
		}
		else
		{
			if(IDNO==AfxMessageBox("未导入应力Excel文件,是否继续生成监测方案?",MB_YESNO))
			{
				return;
			}
		}
	}
	else
	{
		if(ValCount0==0)
		{
			if(IDNO==AfxMessageBox("未导入位移Excel文件,是否继续生成监测方案?",MB_YESNO))
			{
				return;
			}
		}
	}
	int L1_L6[5][6];
	memset(L1_L6,0,sizeof(L1_L6)/sizeof(L1_L6[0][0])*sizeof(int));
	int i=0,j=0,k=0;
	int temp1;
	switch(GradeID)
	{
	case 5:
		{
			for(i=0;i<5;i++)
			{
				for(j=0;j<6;j++)
				{
					k=j/3;
					temp1=int(n/4.0+0.5);
					if(!k)
						L1_L6[i][j] = (m+1)*(n+1)*2+n*(m+1)*(r-1)+m*(n+1)*(r-1)+int(m/4.0*(j+1)+0.5)+m*(temp1>n?n:temp1)*i;
					else
						L1_L6[i][j] = (m+1)*(n+1)*(r-3)+n*(m+1)*(r-1)+m*(n+1)*(r-1)+int(m/4.0*(j-2)+0.5)+m*(temp1>n?n:temp1)*i;
				}
			}
		}
		break;
	case 3:
		{
			for(i=0;i<3;i++)
			{
				for(j=0;j<6;j++)
				{
					k=j/3;
					temp1=int(n/4.0+0.5);
					if(!k)
						L1_L6[i][j] = (m+1)*(n+1)*2+n*(m+1)*(r-1)+m*(n+1)*(r-1)+int(m/4.0*(j+1)+0.5)+m*(temp1>n?n:temp1)*i;
					else
						L1_L6[i][j] = (m+1)*(n+1)*(r-3)+n*(m+1)*(r-1)+m*(n+1)*(r-1)+int(m/4.0*(j-2)+0.5)+m*(temp1>n?n:temp1)*i;
				}
			}		 
		}
		break;
	case 2:
		{
			for(i=1;i<3;i++)
			{
				for(j=0;j<6;j++)
				{
					k=j/3;
					temp1=int(n/4.0+0.5);
					if(!k)
						L1_L6[i][j] = (m+1)*(n+1)*2+n*(m+1)*(r-1)+m*(n+1)*(r-1)+int(m/4.0*(j+1)+0.5)+m*(temp1>n?n:temp1)*i;
					else
						L1_L6[i][j] = (m+1)*(n+1)*(r-3)+n*(m+1)*(r-1)+m*(n+1)*(r-1)+int(m/4.0*(j-2)+0.5)+m*(temp1>n?n:temp1)*i;
				}
			}	 
		}
		break;
	default:break;
	}
	
	int L1_L3[2][3];
	memset(L1_L3,0,sizeof(L1_L3)/sizeof(L1_L3[0][0])*sizeof(int));
	for(i=0;i<2;i++)
	{
		for(j=0;j<3;j++)
		{
			temp1=int(n/2.0*j+0.5);
			if(temp1==0)
				temp1=1;
			if(i==0)
				L1_L3[i][j]=m*(n+1)*(r-1)+n*(m+1)*(r-2)+(temp1>n?n:temp1);
			else
				L1_L3[i][j]=m*(n+1)*(r-1)+n*(m+1)*(r-1)-n+(temp1>n?n:temp1);
		}
	}
	
	CFileDialog dlg(FALSE,NULL,"*.doc",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"*.doc",NULL);
	dlg.m_ofn.lpstrTitle = _T("导出到..."); 
	int rc= dlg.DoModal();
	if(rc != IDOK)
    {
		if(rc==IDCANCEL)
		{
			//m_Word.CloseApp();
			return;
		}
		AfxMessageBox("保存窗口打开失败！");
		return;
	}
	if(rc == IDOK)
	{
		CWordOffice m_Word;
		m_Word.Create();
		m_Word.AddParagraph("碗扣式满堂支架监测方案\n\n",22,TRUE,"宋体",1,0);
		
		m_Word.AddParagraph("1.应力测点布置方案\n",12,TRUE,"宋体",0,2);
		m_Word.AddParagraph("根据Midas/Civil有限元分析软件计算结果，"
			"布置碗扣式满堂支架应力监测测点布置方案如下所示。\n",12,FALSE,"宋体",0,2);
		CString tempLastCVal ="";
		int tempCount = 0;
		for(i=0;i<ValCount;i++)
		{
			if(tempLastCVal==GetCVal[i])
				tempCount++;
			tempLastCVal=GetCVal[i];
		}
		if(ValCount>0)
		{
			ValCount=ValCount-tempCount;
			tempLastCVal=GetCVal[0];
		}
		m_Word.InsertTable(ValCount*5+1+GradeID*6*5, 5);
		m_Word.SetTableText(m_Word.m_wdTable,1,1,"序号",12,FALSE,"宋体",1);
		m_Word.SetTableText(m_Word.m_wdTable,1,2,"单元编号",12,FALSE,"宋体",1);
		m_Word.SetTableText(m_Word.m_wdTable,1,3,"坐标",12,FALSE,"宋体",1);
		m_Word.SetTableText(m_Word.m_wdTable,1,4,"荷载阶段",12,FALSE,"宋体",1);
		m_Word.SetTableText(m_Word.m_wdTable,1,5,"应力值(MPa)",12,FALSE,"宋体",1);
		CString OrderNum = "";
		for(i=0,j=0;i<ValCount+GradeID*6;i++)	
		{
			OrderNum.Format("%d",i+1);
			//m_Word.SetTableText(m_Word.m_wdTable,i+2,1,OrderNum,12,FALSE,"宋体",1);
			m_Word.CellsMerge(m_Word.m_wdTable,i*5+2,1,i*5+6,1,OrderNum);
			m_Word.CellsMerge(m_Word.m_wdTable,i*5+2,2,i*5+6,2,"单元号");
			m_Word.CellsMerge(m_Word.m_wdTable,i*5+2,3,i*5+6,3,"x,y,z");
			m_Word.SetTableText(m_Word.m_wdTable,i*5+2,4,"第一次预压",12,FALSE,"宋体",1);
			m_Word.SetTableText(m_Word.m_wdTable,i*5+3,4,"第二次预压",12,FALSE,"宋体",1);
			m_Word.SetTableText(m_Word.m_wdTable,i*5+4,4,"第三次预压",12,FALSE,"宋体",1);
			m_Word.SetTableText(m_Word.m_wdTable,i*5+5,4,"第一次浇筑",12,FALSE,"宋体",1);
			m_Word.SetTableText(m_Word.m_wdTable,i*5+6,4,"第二次浇筑",12,FALSE,"宋体",1);
			//continue;
			if(i<ValCount)
			{
				for(;j<ValCount+tempCount;)
				{
					if(tempLastCVal==GetCVal[j])
					{
						m_Word.SetTableText(m_Word.m_wdTable,i*5+2,2,tempLastCVal,12,FALSE,"宋体",1);
						m_Word.SetTableText(m_Word.m_wdTable,i*5+2,3,GetXYZ(atoi(tempLastCVal),m,n,r),12,FALSE,"宋体",1);
						if(GetDVal[j]=="第一次预压")
							m_Word.SetTableText(m_Word.m_wdTable,i*5+2,5,GetJVal[j],12,FALSE,"宋体",1);
						else if(GetDVal[j]=="第二次预压")
							m_Word.SetTableText(m_Word.m_wdTable,i*5+3,5,GetJVal[j],12,FALSE,"宋体",1);
						else if(GetDVal[j]=="第三次预压")
							m_Word.SetTableText(m_Word.m_wdTable,i*5+4,5,GetJVal[j],12,FALSE,"宋体",1);
						else if(GetDVal[j]=="第一次浇筑")
							m_Word.SetTableText(m_Word.m_wdTable,i*5+5,5,GetJVal[j],12,FALSE,"宋体",1);
						else if(GetDVal[j]=="第二次浇筑")
							m_Word.SetTableText(m_Word.m_wdTable,i*5+6,5,GetJVal[j],12,FALSE,"宋体",1);
						else
						{};
						j++;
					}
					else
					{
						tempLastCVal=GetCVal[j];
						break;
					}
				}
				//m_Word.SetTableText(m_Word.m_wdTable,i+2,3,"x,y,z",12,FALSE,"宋体",1);
				//m_Word.SetTableText(m_Word.m_wdTable,i+2,4,GetDVal[i],12,FALSE,"宋体",1);
				//m_Word.SetTableText(m_Word.m_wdTable,i+2,5,GetJVal[i],12,FALSE,"宋体",1);
			}
			else
			{
				OrderNum.Format("%d",L1_L6[(i-ValCount)/6][(i-ValCount)%6]);
				m_Word.SetTableText(m_Word.m_wdTable,i*5+2,2,OrderNum,12,FALSE,"宋体",1);
				m_Word.SetTableText(m_Word.m_wdTable,i*5+2,3,GetXYZ(atoi(OrderNum),m,n,r),12,FALSE,"宋体",1);
			}
		}
		m_Word.SetColWidth(m_Word.m_wdTable,1,8.5);
		m_Word.SetColWidth(m_Word.m_wdTable,2,12.9);
		m_Word.SetColWidth(m_Word.m_wdTable,3,12.9);
		m_Word.SetColWidth(m_Word.m_wdTable,4,15.2);
		
		tempLastCVal ="";
		tempCount = 0;
		for(i=0;i<ValCount0;i++)
		{
			if(tempLastCVal==GetCVal0[i])
				tempCount++;
			tempLastCVal=GetCVal0[i];
		}
		if(ValCount0>0)
		{
			ValCount0=ValCount0-tempCount;
			tempLastCVal=GetCVal0[0];
		}
		
		m_Word.AddParagraph("2.位移测点布置方案\n",12,TRUE,"宋体",0,2);
		m_Word.AddParagraph("根据Midas/Civil有限元分析软件计算结果，"
			"布置碗扣式满堂支架应力监测测点布置方案如下所示。\n",12,FALSE,"宋体",0,2);
		
		m_Word.InsertTable(ValCount0*5+1+6*5, 7);
		m_Word.SetTableText(m_Word.m_wdTable,1,1,"序号",12,FALSE,"宋体",1);
		m_Word.SetTableText(m_Word.m_wdTable,1,2,"单元编号",12,FALSE,"宋体",1);
		m_Word.SetTableText(m_Word.m_wdTable,1,3,"位置",12,FALSE,"宋体",1);
		m_Word.SetTableText(m_Word.m_wdTable,1,4,"荷载阶段",12,FALSE,"宋体",1);
		m_Word.SetTableText(m_Word.m_wdTable,1,5,"Dx(mm)",12,FALSE,"宋体",1);
		m_Word.SetTableText(m_Word.m_wdTable,1,6,"Dy(mm)",12,FALSE,"宋体",1);
		m_Word.SetTableText(m_Word.m_wdTable,1,7,"Dz(mm)",12,FALSE,"宋体",1);
		
		for(i=0,j=0;i<ValCount0+6;i++)	
		{
			OrderNum = "";
			OrderNum.Format("%d",i+1);
			//m_Word.SetTableText(m_Word.m_wdTable,i+2,1,OrderNum,12,FALSE,"宋体",1);
			m_Word.CellsMerge(m_Word.m_wdTable,i*5+2,1,i*5+6,1,OrderNum);
			m_Word.CellsMerge(m_Word.m_wdTable,i*5+2,2,i*5+6,2,"单元号");
			m_Word.CellsMerge(m_Word.m_wdTable,i*5+2,3,i*5+6,3,"x,y,z");
			m_Word.SetTableText(m_Word.m_wdTable,i*5+2,4,"第一次预压",12,FALSE,"宋体",1);
			m_Word.SetTableText(m_Word.m_wdTable,i*5+3,4,"第二次预压",12,FALSE,"宋体",1);
			m_Word.SetTableText(m_Word.m_wdTable,i*5+4,4,"第三次预压",12,FALSE,"宋体",1);
			m_Word.SetTableText(m_Word.m_wdTable,i*5+5,4,"第一次浇筑",12,FALSE,"宋体",1);
			m_Word.SetTableText(m_Word.m_wdTable,i*5+6,4,"第二次浇筑",12,FALSE,"宋体",1);
			//continue;
			if(i<ValCount0)
			{
				for(;j<ValCount0+tempCount;)
				{
					if(tempLastCVal==GetCVal0[j])
					{
						m_Word.SetTableText(m_Word.m_wdTable,i*5+2,2,tempLastCVal,12,FALSE,"宋体",1);
						m_Word.SetTableText(m_Word.m_wdTable,i*5+2,3,GetXYZ(atoi(tempLastCVal),m,n,r),12,FALSE,"宋体",1);
						if(GetDVal0[j]=="第一次预压")
						{
							m_Word.SetTableText(m_Word.m_wdTable,i*5+2,5,GetHVal0[j],12,FALSE,"宋体",1);
							m_Word.SetTableText(m_Word.m_wdTable,i*5+2,6,GetIVal0[j],12,FALSE,"宋体",1);
							m_Word.SetTableText(m_Word.m_wdTable,i*5+2,7,GetJVal0[j],12,FALSE,"宋体",1);
						}
						else if(GetDVal0[j]=="第二次预压")
						{
							m_Word.SetTableText(m_Word.m_wdTable,i*5+3,5,GetHVal0[j],12,FALSE,"宋体",1);
							m_Word.SetTableText(m_Word.m_wdTable,i*5+3,6,GetIVal0[j],12,FALSE,"宋体",1);
							m_Word.SetTableText(m_Word.m_wdTable,i*5+3,7,GetJVal0[j],12,FALSE,"宋体",1);
						}
						else if(GetDVal0[j]=="第三次预压")
						{
							m_Word.SetTableText(m_Word.m_wdTable,i*5+4,5,GetHVal0[j],12,FALSE,"宋体",1);
							m_Word.SetTableText(m_Word.m_wdTable,i*5+4,6,GetIVal0[j],12,FALSE,"宋体",1);
							m_Word.SetTableText(m_Word.m_wdTable,i*5+4,7,GetJVal0[j],12,FALSE,"宋体",1);
						}
						else if(GetDVal0[j]=="第一次浇筑")
						{
							m_Word.SetTableText(m_Word.m_wdTable,i*5+5,5,GetHVal0[j],12,FALSE,"宋体",1);
							m_Word.SetTableText(m_Word.m_wdTable,i*5+5,6,GetIVal0[j],12,FALSE,"宋体",1);
							m_Word.SetTableText(m_Word.m_wdTable,i*5+5,7,GetJVal0[j],12,FALSE,"宋体",1);
						}
						else if(GetDVal0[j]=="第二次浇筑")
						{
							m_Word.SetTableText(m_Word.m_wdTable,i*5+6,5,GetHVal0[j],12,FALSE,"宋体",1);
							m_Word.SetTableText(m_Word.m_wdTable,i*5+6,6,GetIVal0[j],12,FALSE,"宋体",1);
							m_Word.SetTableText(m_Word.m_wdTable,i*5+6,7,GetJVal0[j],12,FALSE,"宋体",1);
						}
						else
						{};
						j++;
					}
					else
					{
						tempLastCVal=GetCVal0[j];
						break;
					}
				}
				//m_Word.SetTableText(m_Word.m_wdTable,i+2,3,"x,y,z",12,FALSE,"宋体",1);
				//m_Word.SetTableText(m_Word.m_wdTable,i+2,4,GetDVal[i],12,FALSE,"宋体",1);
				//m_Word.SetTableText(m_Word.m_wdTable,i+2,5,GetJVal[i],12,FALSE,"宋体",1);
			}
			else
			{
				OrderNum.Format("%d",L1_L3[(i-ValCount0)/3][(i-ValCount0)%3]);
				m_Word.SetTableText(m_Word.m_wdTable,i*5+2,2,OrderNum,12,FALSE,"宋体",1);
				m_Word.SetTableText(m_Word.m_wdTable,i*5+2,3,GetXYZ(atoi(OrderNum),m,n,r),12,FALSE,"宋体",1);
			}
		}		
		m_Word.SetColWidth(m_Word.m_wdTable,1,8.5);
		m_Word.SetColWidth(m_Word.m_wdTable,2,12.9);
		m_Word.SetColWidth(m_Word.m_wdTable,3,12.9);
		m_Word.SetColWidth(m_Word.m_wdTable,4,16.0);
		
		
		if(m_Word.SaveDocumentAs(dlg.GetPathName()))
		{
		/*			if(IDOK==AfxMessageBox("导出成功！是否立即打开?",MB_YESNO))
		{
		m_Word.ShowApp();
		m_Word.OpenDocument(dlg.GetPathName());
		return;
		}*/
			AfxMessageBox("导出成功！");
		}
		else
			AfxMessageBox("导出失败！");
		m_Word.CloseApp();
	}
	
	
}

CString CSet::GetXYZ(int Num,int m,int n,int r)
{
	int x,y,z;
	CString result = "";
	if(Num<=m*(n+1)*(r-1))
	{
		x=((int)(ceil((0.0+Num)/m)))%(n+1);
		if(x==0)
			x=n+1;
		z=ceil((ceil((0.0+Num)/m))/(n+1.0));
		y=Num-((z-1)*(n+1)+x-1)*m;
		result.Format("%d,%d,%d\ny向",x,y,z);
		return result;
		
	}
	if(Num<=m*(n+1)*(r-1)+n*(m+1)*(r-1))
	{
		y=((int)(ceil(((Num-m*(n+1.0)*(r-1.0)))/n)))%(m+1);
		if(y==0)
			y=m+1;
		z=ceil((ceil(((Num-m*(n+1.0)*(r-1.0)))/n))/(m+1.0));
		x=Num-m*(n+1)*(r-1)-((z-1)*(m+1)+y-1)*n;
		result.Format("%d,%d,%d\nx向",x,y,z);
		return result;
		
	}
	else
	{
		z=ceil(((Num-m*(n+1.0)*(r-1.0)-n*(m+1)*(r-1)))/((n+1)*(m+1)));
		y=(Num-m*(n+1)*(r-1)-n*(m+1)*(r-1)-(z-1)*(m+1)*(n+1))%m;
		if(y==0)
			y=m+1;
		//ceil((ceil(((float)(Num-m*(n+1)*(r-1)))/n))/((float)(m+1)));
		x=ceil(((Num-m*(n+1.0)*(r-1.0)-n*(m+1)*(r-1)-(z-1)*(m+1)*(n+1)))/m);
		result.Format("%d,%d,%d\nz向",x,y,z);
		return result;
	}
	
}

void CSet::OnButtonInputexcel2() 
{
	CFileDialog file(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("EXCEL文件t(*.xls;*.xlsx)|*.xls;*.xlsx||"),AfxGetMainWnd());
	file.m_ofn.lpstrTitle = _T("导入位移文件...");
	if(file.DoModal()!=IDOK)
	{   
		AfxMessageBox("选择窗口打开失败");
		return;
	}
	CString strFile=file.GetPathName();
	m_Path.SetWindowText(strFile);
	int i=0,j=0,k=0;
	CString tempstr = "";
	vector < CString > CellsVal(20);
	CellsVal.resize(20);
	//CellsVal[0]="123";
	//memset(CellsVal,0,sizeof(CellsVal)/sizeof(CellsVal[0][0]));
	//GetRangeVal(&strFile,NULL,"J3","J20",&CellsVal);
	for(i=0;i<20;i++)
	{
		tempstr = tempstr+CellsVal[i]+"||";
	}
	tempstr = "";
	//AfxMessageBox(tempstr);
	//return;
	_ApplicationE app;
    _Workbook book;
    _Worksheet sheet;
    Workbooks books;
    Worksheets sheets;
    Range range;
    LPDISPATCH lpDisp;
    COleVariant vResult;
    COleVariant covTrue((short)TRUE);
    COleVariant covFalse((short)FALSE);
    COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);	
    //创建Excel服务器(启动Excel)
    if(!app.CreateDispatch("Excel.Application"))
    {
        AfxMessageBox("无法启动Excel服务器!");
        return;
    }
    app.SetVisible(FALSE);          //使Excel不可见	
	books.AttachDispatch(app.GetWorkbooks());
	//CString strFile = "E:\\Desktop\\梁单元应力0611.xlsx";
    lpDisp = books.Open(strFile, 
        covOptional, covFalse, covOptional, covOptional, covOptional,
        covOptional, covOptional, covOptional, covOptional, covOptional,
        covOptional, covOptional, covOptional, covOptional);
	
    //得到Workbook
    book.AttachDispatch(lpDisp);
	//CString bookname = book.GetName();
	
    //得到Worksheets
	//CString tempstr = "梁单元应力";
	//sheets.Select(COleVariant(tempstr));
    sheets.AttachDispatch(book.GetWorksheets());//
	for(i=0;;i++)
	{
		lpDisp=sheets.GetItem(COleVariant(short(i+1)));//book.GetActiveSheet();
		sheet.AttachDispatch(lpDisp);
		if(sheet.GetName()=="Sheet1")
			break;	
	}
	
	int SheetNum = i;
	int m_ValCount0[8];
	memset(m_ValCount0,0,sizeof(m_ValCount0)/sizeof(m_ValCount0[0])*sizeof(int));
	Range usedRange;
	long RowNum=0;
	long ColNum=0;
	CString EndCellCode = "";
	VARIANT key1; // these lines set up first arg (key1) to sort
	Range iRange;
	CString tempSCell = "";
	CString tempECell = "";
	VARIANT varRead ;
	COleSafeArray olesaRead;	//得到当前活跃sheet
	CString minHcodeCVal0;
	CString maxHcodeCVal0 ;
	CString minIcodeCVal0;
	CString maxIcodeCVal0;
	CString minJcodeCVal0;
	int SRow = 0;
	GetCVal0.clear();
	GetDVal0.clear();
	GetHVal0.clear();
	GetIVal0.clear();
	GetJVal0.clear();
	GetCVal0.resize(60);
	GetDVal0.resize(60);
	GetHVal0.resize(60);
	GetIVal0.resize(60);
	GetJVal0.resize(60);
	ValCount0=0;
	vector < vector < CString > > m_GetCVal0;
	vector < vector < CString > > m_GetDVal0;
	vector < vector < CString > > m_GetHVal0;
	vector < vector < CString > > m_GetIVal0;
	vector < vector < CString > > m_GetJVal0;
	m_GetCVal0.clear();
	m_GetDVal0.clear();
	m_GetHVal0.clear();
	m_GetIVal0.clear();
	m_GetJVal0.clear();
	m_GetCVal0.resize(SheetNum);
	m_GetDVal0.resize(SheetNum);
	m_GetHVal0.resize(SheetNum);
	m_GetIVal0.resize(SheetNum);
	m_GetJVal0.resize(SheetNum);
	for(i=0;i<SheetNum;i++)
	{
		m_GetCVal0[i].resize(40);
	}
	for(i=0;i<SheetNum;i++)
	{
		m_GetDVal0[i].resize(40);
	}
	for(i=0;i<SheetNum;i++)
	{
		m_GetHVal0[i].resize(40);
	}
	for(i=0;i<SheetNum;i++)
	{
		m_GetIVal0[i].resize(40);
	}
	for(i=0;i<SheetNum;i++)
	{
		m_GetJVal0[i].resize(40);
	}
	
	//如果有单元格正处于编辑状态中，此操作不能返回，会一直等待
	SetProcess(0);
	for(i=0;i<SheetNum;i++)
	{
		lpDisp=sheets.GetItem(COleVariant(short(i+1)));//book.GetActiveSheet();
		sheet.AttachDispatch(lpDisp);
		//CString sheetname = sheet.GetName();
		
		//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
		usedRange.AttachDispatch(sheet.GetUsedRange());
		range.AttachDispatch(usedRange.GetRows());
		RowNum=range.GetCount();                   //已经使用的行数
		range.ReleaseDispatch();
		range.AttachDispatch(usedRange.GetColumns());	
		ColNum=range.GetCount()-1;               //获取有效列数目，获取结果总比实际多1
		EndCellCode.Format("%s%d",TranslateColName(ColNum),RowNum);
		
		V_VT(&key1) = VT_DISPATCH;                           // 排序时，关键字的vt设置为VT_DISPATCH
		
		V_DISPATCH(&key1) = sheet.GetRange(COleVariant("H1"),COleVariant("H1"));   // 设置按哪个关键字进行排序 选择单元意为：按此列（或行）为主关键字进行排序	
		//range=sheet.GetRange(COleVariant(L"A1"),COleVariant(CellName));              // 选择对哪些区域内的单元格进行排序
		if(i==0)// 选择对哪些区域内的单元格进行排序
		{
			iRange = sheet.GetRange(COleVariant("C3"),COleVariant(EndCellCode));
			tempSCell.Format("C3");
			tempECell.Format("C4");
		}
		else
		{
			iRange = sheet.GetRange(COleVariant("C2"),COleVariant(EndCellCode));
			tempSCell.Format("C2");
			tempECell.Format("C3");
		}
		iRange.Sort(key1, xlAscending, vOpt, vOpt, xlAscending, vOpt,xlAscending,
			xlNoHeader,vOpt,xlIgnoreCase,xlTopToBottom,xlPinYin,0,0,0); //设置排序 无标题时一定要设置为xlNoHeader  否则不起作用
		
		lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
		iRange.AttachDispatch(lpDisp);
		varRead = iRange.GetValue2();
		olesaRead.Attach(varRead);
		GetValFromArry(&olesaRead,&CellsVal);
		olesaRead.Detach();
		minHcodeCVal0 = CellsVal[0];
		//AfxMessageBox(minHcodeCVal0);
		
		
		tempSCell.Format("C%d",RowNum);
		tempECell.Format("C%d",RowNum+1);
		lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
		iRange.AttachDispatch(lpDisp);
		olesaRead.Clear();
		varRead = iRange.GetValue2();
		olesaRead.Attach(varRead);
		GetValFromArry(&olesaRead,&CellsVal);
		maxHcodeCVal0 = CellsVal[0];
		//AfxMessageBox(maxJcodeCVal);
		/*	V_DISPATCH(&key1) = sheet.GetRange(COleVariant("C1"),COleVariant("C1"));   // 设置按哪个关键字进行排序 选择单元意为：按此列（或行）为主关键字进行排序	
		lpDisp = sheet.GetRange(COleVariant("C3"), COleVariant(EndCellCode));
		iRange.AttachDispatch(lpDisp);
		iRange.Sort(key1, xlAscending, vOpt, vOpt, xlAscending, vOpt,xlAscending,
		xlNoHeader,vOpt,xlIgnoreCase,xlTopToBottom,xlPinYin,0,0,0);*/ //设置排序 无标题时一定要设置为xlNoHeader  否则不起作用
		/////////////////////////////////////////////////////////////////////////////////////////
		
		V_DISPATCH(&key1) = sheet.GetRange(COleVariant("I1"),COleVariant("I1"));   // 设置按哪个关键字进行排序 选择单元意为：按此列（或行）为主关键字进行排序	
		//range=sheet.GetRange(COleVariant(L"A1"),COleVariant(CellName));              // 选择对哪些区域内的单元格进行排序
		if(i==0)
		{
			lpDisp = sheet.GetRange(COleVariant("C3"), COleVariant(EndCellCode));
			tempSCell.Format("C3");
			tempECell.Format("C4");
		}
		else
		{
			lpDisp = sheet.GetRange(COleVariant("C2"), COleVariant(EndCellCode));
			tempSCell.Format("C2");
			tempECell.Format("C3");
		}
		iRange.AttachDispatch(lpDisp);
		iRange.Sort(key1, xlAscending, vOpt, vOpt, xlAscending, vOpt,xlAscending,
			xlNoHeader,vOpt,xlIgnoreCase,xlTopToBottom,xlPinYin,0,0,0); //设置排序 无标题时一定要设置为xlNoHeader  否则不起作用
		
		lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
		iRange.AttachDispatch(lpDisp);
		varRead = iRange.GetValue2();
		olesaRead.Attach(varRead);
		GetValFromArry(&olesaRead,&CellsVal);
		olesaRead.Detach();
		minIcodeCVal0 = CellsVal[0];
		//AfxMessageBox(minJcodeCVal);
		
		
		tempSCell.Format("C%d",RowNum);
		tempECell.Format("C%d",RowNum+1);
		lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
		iRange.AttachDispatch(lpDisp);
		olesaRead.Clear();
		varRead = iRange.GetValue2();
		olesaRead.Attach(varRead);
		GetValFromArry(&olesaRead,&CellsVal);
		maxIcodeCVal0 = CellsVal[0];
		//AfxMessageBox(maxJcodeCVal);
		/*	V_DISPATCH(&key1) = sheet.GetRange(COleVariant("C1"),COleVariant("C1"));   // 设置按哪个关键字进行排序 选择单元意为：按此列（或行）为主关键字进行排序	
		lpDisp = sheet.GetRange(COleVariant("C3"), COleVariant(EndCellCode));
		iRange.AttachDispatch(lpDisp);
		iRange.Sort(key1, xlAscending, vOpt, vOpt, xlAscending, vOpt,xlAscending,
		xlNoHeader,vOpt,xlIgnoreCase,xlTopToBottom,xlPinYin,0,0,0);*/ //设置排序 无标题时一定要设置为xlNoHeader  否则不起作用
		///////////////////////////////////////////////////////////////////////////////////
		
		V_DISPATCH(&key1) = sheet.GetRange(COleVariant("J1"),COleVariant("J1"));   // 设置按哪个关键字进行排序 选择单元意为：按此列（或行）为主关键字进行排序	
		//range=sheet.GetRange(COleVariant(L"A1"),COleVariant(CellName)); 
		if(i==0)// 选择对哪些区域内的单元格进行排序
		{
			iRange = sheet.GetRange(COleVariant("C3"),COleVariant(EndCellCode));
			tempSCell.Format("C3");
			tempECell.Format("C4");
		}
		else
		{
			iRange = sheet.GetRange(COleVariant("C2"),COleVariant(EndCellCode));
			tempSCell.Format("C2");
			tempECell.Format("C3");
		}
		iRange.Sort(key1, xlAscending, vOpt, vOpt, xlAscending, vOpt,xlAscending,
			xlNoHeader,vOpt,xlIgnoreCase,xlTopToBottom,xlPinYin,0,0,0); //设置排序 无标题时一定要设置为xlNoHeader  否则不起作用
		
		lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
		iRange.AttachDispatch(lpDisp);
		varRead = iRange.GetValue2();
		olesaRead.Attach(varRead);
		GetValFromArry(&olesaRead,&CellsVal);
		olesaRead.Detach();
		minJcodeCVal0 = CellsVal[0];
		//AfxMessageBox(minJcodeCVal);
		
		
		tempSCell.Format("C%d",RowNum);
		tempECell.Format("C%d",RowNum+1);
		lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
		iRange.AttachDispatch(lpDisp);
		olesaRead.Clear();
		varRead = iRange.GetValue2();
		olesaRead.Attach(varRead);
		GetValFromArry(&olesaRead,&CellsVal);
		CString maxJcodeCVal0 = CellsVal[0];
		//AfxMessageBox(maxJcodeCVal0);
		V_DISPATCH(&key1) = sheet.GetRange(COleVariant("C1"),COleVariant("C1"));   // 设置按哪个关键字进行排序 选择单元意为：按此列（或行）为主关键字进行排序	
		if(i==0)
			lpDisp = sheet.GetRange(COleVariant("C3"), COleVariant(EndCellCode));
		else
			lpDisp = sheet.GetRange(COleVariant("C2"), COleVariant(EndCellCode));
		iRange.AttachDispatch(lpDisp);
		iRange.Sort(key1, xlAscending, vOpt, vOpt, xlAscending, vOpt,xlAscending,
			xlNoHeader,vOpt,xlIgnoreCase,xlTopToBottom,xlPinYin,0,0,0); //设置排序 无标题时一定要设置为xlNoHeader  否则不起作用
		///////////////////////////////////////////////////////////////////////////////////
		//tempstr.Format("%d",RowNum);
		//AfxMessageBox(tempstr);
		if(i==0)
			SRow = 3;
		else
			SRow = 2;
		for(SRow=3;SRow<=RowNum;SRow++)
		{
			SetProcess((i*RowNum+SRow)/(SheetNum*(RowNum+1.0))*100);
			tempSCell.Format("C%d",SRow);
			tempECell.Format("C%d",SRow+1);
			lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
			iRange.AttachDispatch(lpDisp);
			varRead = iRange.GetValue2();
			olesaRead.Attach(varRead);
			GetValFromArry(&olesaRead,&CellsVal);
			olesaRead.Detach();
			if((minHcodeCVal0 == CellsVal[0])||(maxHcodeCVal0 == CellsVal[0])||(minIcodeCVal0 == CellsVal[0])||(maxIcodeCVal0 == CellsVal[0])||(minJcodeCVal0 == CellsVal[0])||(maxJcodeCVal0 == CellsVal[0]))
			{
				m_GetCVal0[i][m_ValCount0[i]].Format("%d",atoi(CellsVal[0]));
				//AfxMessageBox(minJcodeCVal0+maxJcodeCVal0);
				//return;
				tempSCell.Format("D%d",SRow);
				tempECell.Format("D%d",SRow+1);
				lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
				iRange.AttachDispatch(lpDisp);
				varRead = iRange.GetValue2();
				olesaRead.Attach(varRead);
				GetValFromArry(&olesaRead,&CellsVal);
				olesaRead.Detach();
				m_GetDVal0[i][m_ValCount0[i]]=CellsVal[0];
				
				tempSCell.Format("H%d",SRow);
				tempECell.Format("H%d",SRow+1);
				lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
				iRange.AttachDispatch(lpDisp);
				varRead = iRange.GetValue2();
				olesaRead.Attach(varRead);
				GetValFromArry(&olesaRead,&CellsVal);
				olesaRead.Detach();
				m_GetHVal0[i][m_ValCount0[i]]=CellsVal[0];
				
				tempSCell.Format("I%d",SRow);
				tempECell.Format("I%d",SRow+1);
				lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
				iRange.AttachDispatch(lpDisp);
				varRead = iRange.GetValue2();
				olesaRead.Attach(varRead);
				GetValFromArry(&olesaRead,&CellsVal);
				olesaRead.Detach();
				m_GetIVal0[i][m_ValCount0[i]] = CellsVal[0];
				
				tempSCell.Format("J%d",SRow);
				tempECell.Format("J%d",SRow+1);
				lpDisp = sheet.GetRange(COleVariant(tempSCell), COleVariant(tempECell));
				iRange.AttachDispatch(lpDisp);
				varRead = iRange.GetValue2();
				olesaRead.Attach(varRead);
				GetValFromArry(&olesaRead,&CellsVal);
				olesaRead.Detach();
				m_GetJVal0[i][m_ValCount0[i]++] = CellsVal[0];
				
			}
		}
	}
	
	
	//book.Save();
    book.Close(covFalse,COleVariant(strFile),covOptional);
    books.Close();      
    app.Quit();
	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app.ReleaseDispatch();
	CString FinalHMax = m_GetHVal0[0][0];
	CString FinalIMax = m_GetIVal0[0][0];
	CString FinalJMax = m_GetJVal0[0][0];
	//double tempdouble = 0.0;
	for(j=0;j<SheetNum;j++)
	{
		for(i=0;i<m_ValCount0[j];i++)
		{
			//FinalVal[j][i]=atof(m_GetJVal[j][i]);
			FinalHMax = fabs(atof(m_GetHVal0[j][i]))>fabs(atof(FinalHMax))?m_GetHVal0[j][i]:FinalHMax;
			FinalIMax = fabs(atof(m_GetIVal0[j][i]))>fabs(atof(FinalIMax))?m_GetIVal0[j][i]:FinalIMax;
			FinalJMax = fabs(atof(m_GetJVal0[j][i]))>fabs(atof(FinalJMax))?m_GetJVal0[j][i]:FinalJMax;
			//FinalMax = atof(FinalMax)>atof(m_GetJVal[j][i])?FinalMax:m_GetJVal[j][i];
			//FinalMin = atof(FinalMin)<atof(m_GetJVal[j][i])?FinalMin:m_GetJVal[j][i];
			tempstr = tempstr+ m_GetCVal0[j][i]+","+m_GetDVal0[j][i]+","+m_GetHVal0[j][i]+","+m_GetIVal0[j][i]+","+m_GetJVal0[j][i]+"\n";
		}
		tempstr = tempstr+"\n";
	}
	//	AfxMessageBox(tempstr);
	tempstr = FinalHMax+"\n"+FinalIMax+"\n"+FinalJMax;
	//AfxMessageBox(tempstr);
	int l=0;
	ValCount0 = 0;
	for(j=0;j<SheetNum;j++)
	{
		for(i=0;i<m_ValCount0[j];i++)
		{
			if(m_GetHVal0[j][i]==FinalHMax||m_GetIVal0[j][i]==FinalIMax||m_GetJVal0[j][i]==FinalJMax)
			{
				//m_GetCVal[j][i]
				for(k=0;k<SheetNum;k++)
				{
					for(l=0;l<m_ValCount0[k];l++)
					{
						if(m_GetCVal0[j][i]==m_GetCVal0[k][l])//||m_GetIVal0[j][i]==m_GetIVal0[k][l]||m_GetJVal0[j][i]==m_GetJVal0[k][l])
						{
							//CString tempi = "";
							
							//tempi.Format("ValCount0 = %d",ValCount0);
							//AfxMessageBox(tempi);
							GetCVal0[ValCount0] = m_GetCVal0[k][l];
							GetDVal0[ValCount0] = m_GetDVal0[k][l];
							GetHVal0[ValCount0] = m_GetHVal0[k][l];
							GetIVal0[ValCount0] = m_GetHVal0[k][l];
							GetJVal0[ValCount0++] = m_GetJVal0[k][l];
						}
					}
				}
			}
		}
	}
	tempstr = "";
	for(i=0;i<ValCount0;i++)
	{
		tempstr = tempstr+ GetCVal0[i]+","+GetDVal0[i]+","+GetHVal0[i]+","+GetIVal0[i]+","+GetJVal0[i]+"\n";
	}
	//AfxMessageBox(tempstr);
	AfxMessageBox("读取位移数据成功！");
	
	return;
}


void CSet::OnButtonCaclu() 
{
	CWinThread *pThread=AfxBeginThread(RUNTIME_CLASS(CUIThread));
}




void CSet::SetProcess(int process)
{
	//UpdateData(TRUE);
	//Info.pctrlProgress=&m_ProgressTest;
	//创建MFC线程
	//CProgressCtrl* lpctrlProgress=
	/*Info.pctrlProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	Process=process;
	UpdateData(TRUE);
	AfxBeginThread(ThreadFunc,&Info);*/
	
}

void CSet::OnChangeZhuJuX1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CSet::OnChangePaiJuY() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CSet::OnButton1() 
{
	int BB,BB1,ZZ,ZZ1,PP,PP1;
    Deal_PaiJu_Y();
    Deal_ZhuJu_X();
    Deal_BuJu_Z();
	int tempBu=0;
	int tempZhu=0;
	int tempPai=0;
	CString filename="h:\\midas.txt";
	//filename=filename+".mct";
	CStdioFile File;
	File.Open(filename,CFile::modeCreate|CFile::modeReadWrite);//如果文件事先不存在的话，就需要CFile::modeCreate，否则就不需要。
	CString TxtStr="";
	float CurZVal = 0.0;
	for(BB=-1;BB<Count_Z;BB++)
	{
		if(BB==-1)
			tempBu=1;
		else
			tempBu=BuJuCountSave[BB];
		for(BB1=0;BB1<tempBu;BB1++)
		{
			if(BB==-1)
				CurZVal=0.0;
			else
				CurZVal=CurZVal+BuJuDataSave[BB];
			float CurXVal = 0.0;
			for(ZZ=-1;ZZ<Count_X;ZZ++)
			{
				if(ZZ==-1)
					tempZhu=1;
				else
					tempZhu=ZhuJuCountSave[ZZ];
				for(ZZ1=0;ZZ1<tempZhu;ZZ1++)
				{
					if(ZZ==-1)
						CurXVal=0.0;
					else
						CurXVal=CurXVal+ZhuJuDataSave[ZZ];
					float CurYVal=0.0;
					for(PP=-1;PP<Count_Y;PP++)
					{
						if(PP==-1)
							tempPai=1;
						else
							tempPai=PaiJuCountSave[PP];
						for(PP1=0;PP1<tempPai;PP1++)
						{
							if(PP==-1)
								CurYVal=0.0;
							else
								CurYVal=CurYVal+PaiJuDataSave[PP];
							TxtStr.Format("%.2f,%.2f,%.2f\n",CurXVal,CurYVal,CurZVal);
							File.WriteString(TxtStr);
						}
					}
				}
			}
		}
	}
	File.Close();
	AfxMessageBox("完成");
}

void CSet::OnChangeXiaTuoChengGaoDu() 
{
	CString str; 
	UpdateData(TRUE);
	GetDlgItemText(IDE_XiaTuoChengGaoDu,str);
	XiaBuTuoChengGaoDu=atof(str);
	return;
	
}


void CSet::OnAddWaijing() 
{
	CString tempstr;
	CListCtrl* m_list=NULL;
	((CEdit*)GetDlgItem(IDE_WaiJingZhi))->GetWindowText(tempstr);
	double Val=atof(tempstr);
	((CEdit*)GetDlgItem(IDE_WaiJingBaiFenBi))->GetWindowText(tempstr);
	int Per=atoi(tempstr);
	if(Val>2*D)
	{
		AfxMessageBox("误差外径过大");
		return;
	}
	if(Val<0.5*D)
	{
		AfxMessageBox("误差外径过小");
		return;
	}
	if(Val==D)
	{
		AfxMessageBox("误差外径不能与标准外径相同");
		return;
	}
	if(Per<0)
	{
		AfxMessageBox("误差率不能为负数");
		return;
	}
	m_list=(CListCtrl*)GetDlgItem(IDC_LIST_WAIJING_WUCHA);
	int i=0;
	for(i=0;i<m_list->GetItemCount();i++)
	{
		tempstr=m_list->GetItemText(i,0);
		if(atof(tempstr)==Val)
		{
			tempstr.Format("Val与第%d行重复",i+1);
			AfxMessageBox(tempstr);
			return;
		}
	}
	if(Per>20)
	{
		tempstr.Format("误差率过大!");
		MessageBox(tempstr,"警告",MB_ICONWARNING);
		//	return;
	}
	tempstr.Format("%.4f",Val);
	m_list->InsertItem(0,"");
	m_list->SetItemText(0,0,tempstr);
	tempstr.Format("%d",Per);
	m_list->SetItemText(0,1,tempstr);
	
	
}

void CSet::OnDelWaijing() 
{
	CListCtrl* m_list=NULL;
	m_list=(CListCtrl*)GetDlgItem(IDC_LIST_WAIJING_WUCHA);
	while(m_list->GetNextItem(-1,LVNI_ALL | LVNI_SELECTED) != -1)
	{
		int nItem = m_list->GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
		m_list->DeleteItem(nItem);
	}
}

void CSet::OnAddBihou() 
{
	CString tempstr;
	CListCtrl* m_list=NULL;
	((CEdit*)GetDlgItem(IDE_BiHouZhi))->GetWindowText(tempstr);
	double Val=atof(tempstr);
	((CEdit*)GetDlgItem(IDE_BiHouBaiFenBi))->GetWindowText(tempstr);
	int Per=atoi(tempstr);
	if(Val>2*tw)
	{
		AfxMessageBox("误差	壁厚过大");
		return;
	}
	if(Val<0.5*tw)
	{
		AfxMessageBox("误差壁厚过小");
		return;
	}
	if(Val==tw)
	{
		AfxMessageBox("误差壁厚不能与标准壁厚相同");
		return;
	}
	if(Per<0)
	{
		AfxMessageBox("误差率不能为负数");
		return;
	}
	m_list=(CListCtrl*)GetDlgItem(IDC_LIST_BIHOU_WUCHA);
	int i=0;
	for(i=0;i<m_list->GetItemCount();i++)
	{
		tempstr=m_list->GetItemText(i,0);
		if(atof(tempstr)==Val)
		{
			tempstr.Format("Val与第%d行重复",i+1);
			AfxMessageBox(tempstr);
			return;
		}
	}
	if(Per>20)
	{
		tempstr.Format("误差率过大!");
		MessageBox(tempstr,"警告",MB_ICONWARNING);
		//	return;
	}
	tempstr.Format("%.4f",Val);
	m_list->InsertItem(0,"");
	m_list->SetItemText(0,0,tempstr);
	tempstr.Format("%d",Per);
	m_list->SetItemText(0,1,tempstr);	
}

void CSet::OnDelBihou() 
{
	CListCtrl* m_list=NULL;
	m_list=(CListCtrl*)GetDlgItem(IDC_LIST_BIHOU_WUCHA);
	while(m_list->GetNextItem(-1,LVNI_ALL | LVNI_SELECTED) != -1)
	{
		int nItem = m_list->GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
		m_list->DeleteItem(nItem);
	}
}

BOOL CSet::AddDeviation()
{
	CListCtrl* m_list=NULL;
	int i=0;
	int Per=0;
	CString tempstr="";
	m_list=(CListCtrl*)GetDlgItem(IDC_LIST_WAIJING_WUCHA);
	D_Deviation_Per[0]=m_list->GetItemCount()+1;
	if(D_Deviation_Per[0]>=20)
	{
		AfxMessageBox("误差外径值超过18个,请保持低于18个");
		return FALSE;
	}
	for(i=0;i<D_Deviation_Per[0]-1;i++)
	{
		tempstr=m_list->GetItemText(i,0);
		D_Deviation[i+2]=atof(tempstr);
		tempstr=m_list->GetItemText(i,1);
		D_Deviation_Per[i+2]=atoi(tempstr);
		Per=Per+D_Deviation_Per[i+2];
	}
	if(Per>30)
	{
		if(IDYES!=MessageBox("当前误差率超过30%,是否继续?","警告",MB_YESNO))
			return FALSE;
	}
	Per=0;
	m_list=(CListCtrl*)GetDlgItem(IDC_LIST_BIHOU_WUCHA);
	tw_Deviation_Per[0]=m_list->GetItemCount()+1;
	if(tw_Deviation_Per[0]>=20)
	{
		AfxMessageBox("误差壁厚值超过18个,请保持低于18个");
		return FALSE;
	}
	for(i=0;i<tw_Deviation_Per[0]-1;i++)
	{
		tempstr=m_list->GetItemText(i,0);
		tw_Deviation[i+2]=atof(tempstr);
		tempstr=m_list->GetItemText(i,1);
		tw_Deviation_Per[i+2]=atoi(tempstr);
		Per=Per+tw_Deviation_Per[i+2];
	}
	if(Per>30)
	{
		if(IDYES!=MessageBox("当前误差率超过30%,是否继续?","警告",MB_YESNO))
			return FALSE;
	}
	return TRUE;
	
}

void CSet::OnBtnTestzero() 
{
/*typedef double (_stdcall * AddProc)(double,double);//定义函数指针类型

  HINSTANCE hInst;
  
	hInst=LoadLibrary("Zero.dll");//动态加载Dll
	
	  AddProc Max=(AddProc)GetProcAddress(hInst,"max");//获取Dll的导出函数
	  
		if(!Max)
		{
		MessageBox("获取Max函数地址失败！");
		}
		
		  //double guess=Rand::Gauss(0.5,1.5);
		  CString str="";
		  str.Format("guess=%.4f",Max(1.5,2.6));
		  AfxMessageBox(str);
	::FreeLibrary(hInst);//释放Dll函数*/
}

void CSet::OnBtnSection() 
{
	CSection dlg;
	dlg.DoModal();
}

void CSet::InitVal()
{
	SudoINI();
	SectionFlag=0;
	Count_X=0;
	Count_Y=0;
	Count_Z=0;
	GradeID=5;
	ValCount0=0;
	ValCount=0;
	XiaBuTuoChengGaoDu=0;
	//memset(ValCount,0,sizeof(ValCount)/sizeof(ValCount[0])*sizeof(int));
	((CButton*)GetDlgItem(IDC_RADIO_G1))->SetCheck(TRUE);
	CListCtrl* m_list=(CListCtrl*)GetDlgItem(IDC_LIST_WAIJING_WUCHA);
	m_list->InsertColumn(0,"外径",LVCFMT_CENTER,50);
	m_list->InsertColumn(1,"%",LVCFMT_CENTER,30);
	m_list->InsertColumn(2,"ToTal(%)",LVCFMT_CENTER,70);
	m_list->SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	m_list=(CListCtrl*)GetDlgItem(IDC_LIST_BIHOU_WUCHA);
	m_list->InsertColumn(0,"壁厚",LVCFMT_CENTER,50);
	m_list->InsertColumn(1,"%",LVCFMT_CENTER,30);
	m_list->InsertColumn(2,"ToTal(%)",LVCFMT_CENTER,70);
	m_list->SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	PaiJuGeShu_Y=0;
	ZhuJuGeShu_X=0;
	BuJuGeShu_Z=0;	// TODO: Add extra initialization here
	D=0.048;//D为标准外径
	tw=0.0035;//tw为标准壁厚
	CString tempstr="";
	tempstr.Format("%.3f",D);
	((CEdit*)GetDlgItem(IDE_GangGuanWaiJing))->SetWindowText(tempstr);
	tempstr.Format("%.4f",tw);
	((CEdit*)GetDlgItem(IDE_GangGuanBiHou))->SetWindowText(tempstr);
	memset(D_Deviation,0,sizeof(D_Deviation)/sizeof(D_Deviation[0])*sizeof(double));
	memset(D_Deviation_Per,0,sizeof(D_Deviation_Per)/sizeof(D_Deviation_Per[0])*sizeof(int));
	memset(tw_Deviation,0,sizeof(tw_Deviation)/sizeof(tw_Deviation[0])*sizeof(double));
	memset(tw_Deviation_Per,0,sizeof(tw_Deviation_Per)/sizeof(tw_Deviation_Per[0])*sizeof(int));

	int i=0;
	SectionVal[i++]=20;
	SectionVal[i++]=50;
	SectionVal[i++]=110;

	SectionVal[i++]=200;
	SectionVal[i++]=50;
	SectionVal[i++]=250;
	SectionVal[i++]=50;
	SectionVal[i++]=250;
	SectionVal[i++]=50;

	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;

	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
	SectionVal[i++]=30;
}

void CSet::SudoINI()
{
//获取exe路径
    CString  strPath;   
	GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);     
	strPath.ReleaseBuffer();     
	int nPos = strPath.ReverseFind('\\');     
	strPath=strPath.Left(nPos);     
	strPath += "\\initval.ini";
    //向INI文件中添加键值 
	CString tempstr;
	GetPrivateProfileString("初始edit值", "IDE_ZhuJu_X1", "0",  tempstr.GetBuffer(MAX_PATH),MAX_PATH,strPath); 
	SetDlgItemText(IDE_ZhuJu_X1, tempstr);
	tempstr.ReleaseBuffer();
	GetPrivateProfileString("初始edit值", "IDE_PaiJu_Y", "0",  tempstr.GetBuffer(MAX_PATH),MAX_PATH,strPath); 
	SetDlgItemText(IDE_PaiJu_Y, tempstr); 
	tempstr.ReleaseBuffer();
	GetPrivateProfileString("初始edit值", "IDE_BuJu_Z", "0",  tempstr.GetBuffer(MAX_PATH),MAX_PATH,strPath); 
	SetDlgItemText(IDE_BuJu_Z, tempstr); 
	tempstr.ReleaseBuffer();
	GetPrivateProfileString("初始edit值", "IDE_DingCengXuanBi", "0",  tempstr.GetBuffer(MAX_PATH),MAX_PATH,strPath); 
	SetDlgItemText(IDE_DingCengXuanBi, tempstr); 
	tempstr.ReleaseBuffer();
	GetPrivateProfileString("初始edit值", "IDE_SaoDiGanGaoDu", "0",  tempstr.GetBuffer(MAX_PATH),MAX_PATH,strPath); 
	SetDlgItemText(IDE_SaoDiGanGaoDu, tempstr); 
	tempstr.ReleaseBuffer();
	GetPrivateProfileString("初始edit值", "IDE_XiaTuoChengGaoDu", "0",  tempstr.GetBuffer(MAX_PATH),MAX_PATH,strPath); 
	SetDlgItemText(IDE_XiaTuoChengGaoDu, tempstr); 
	tempstr.ReleaseBuffer();
   /*WritePrivateProfileString ("初始edit值", "IDE_ZhuJu_X1", "8@1.2 7@1.6", strPath); 
    WritePrivateProfileString ("初始edit值",  "IDE_PaiJu_Y", "3@2",strPath); 
    WritePrivateProfileString ("初始edit值",  "IDE_BuJu_Z", "9@1.1 8@1.6",strPath); 
    WritePrivateProfileString ("初始edit值",  "IDE_DingCengXuanBi", "0.35",strPath); 
    WritePrivateProfileString ("初始edit值",  "IDE_SaoDiGanGaoDu", "0.30",strPath); 
    WritePrivateProfileString ("初始edit值", "IDE_XiaTuoChengGaoDu", "0.15", strPath); */
}

void CSet::JudgeCross(float x1, float y1, float x2, float y2, float a1, float b1, float a2, float b2,float res[])
{//该函数前四个数为单元坐标，后四个数为剪刀撑坐标
	//sciNode cross;
	//cross.Num=-1;
	res[0]=-1.0;
	res[1]=-1.0;
	if(((b1-y1)*(x1-x2)-(y1-y2)*(a1-x1))*((b2-y1)*(x1-x2)-(y1-y2)*(a2-x1))<=0&&
		((y1-b1)*(a1-a2)-(b1-b2)*(x1-a1))*((y2-b1)*(a1-a2)-(b1-b2)*(x2-a1))<=0)
	{
		if(x1==x2)
		{
			res[0]=x1;
			res[1]=(b1-b2)*(x1-a1)/(a1-a2)+b1;
		}
		else if(y1==y2)
		{
			res[1]=y1;
			res[0]=a1+(y1-b1)*(a1-a2)/(b1-b2);
		}
		else
			res[0]=(x1*(y1-y2)*(a1-a2)-a1*(b1-b2)*(x1-x2)-y1+b1)/((y1-y2)*(a1-a2)-(b1-b2)*(x1-x2));
			res[1]=(b1-b2)*(res[0]-a1)/(a1-a2)+b1;
	}
}
