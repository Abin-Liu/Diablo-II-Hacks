; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CConfigDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ShopbotCfg.h"

ClassCount=7
Class1=CShopbotCfgApp
Class2=CShopbotCfgDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_CFGDLG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CConfigDlg
Resource4=IDD_SHOPBOTCFG_DIALOG
Class5=CModDlg
Resource5=IDD_MODDLG
Class6=CAssistDlg
Resource6=IDD_ASSISTDLG
Class7=CSkillTypeDlg
Resource7=IDD_SKILLTYPEDLG

[CLS:CShopbotCfgApp]
Type=0
HeaderFile=ShopbotCfg.h
ImplementationFile=ShopbotCfg.cpp
Filter=N

[CLS:CShopbotCfgDlg]
Type=0
HeaderFile=ShopbotCfgDlg.h
ImplementationFile=ShopbotCfgDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_WP

[CLS:CAboutDlg]
Type=0
HeaderFile=ShopbotCfgDlg.h
ImplementationFile=ShopbotCfgDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SHOPBOTCFG_DIALOG]
Type=1
Class=CShopbotCfgDlg
ControlCount=20
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_ACT,combobox,1344339971
Control4=IDC_STATIC,static,1342308352
Control5=IDC_NPC,combobox,1344339971
Control6=IDC_STATIC,static,1342308352
Control7=IDC_WP,combobox,1344339971
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_HOTKEY,combobox,1344339971
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STOPHP,edit,1350631552
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,button,1342177287
Control15=IDC_LIST1,SysListView32,1342291977
Control16=IDC_ADD,button,1342242816
Control17=IDC_EDIT,button,1476460544
Control18=IDC_DEL,button,1476460544
Control19=IDOK,button,1476460544
Control20=IDCANCEL,button,1342242816

[DLG:IDD_CFGDLG]
Type=1
Class=CConfigDlg
ControlCount=14
Control1=IDC_STATIC,static,1342308352
Control2=IDC_DESC,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_CODE,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_SOCKETS,combobox,1344339971
Control8=IDC_NONMAGIC,button,1342242819
Control9=IDC_MODS,SysListView32,1350631433
Control10=IDC_ADD,button,1342242816
Control11=IDC_DEL,button,1476460544
Control12=IDC_EDIT,button,1476460544
Control13=IDOK,button,1342242816
Control14=IDCANCEL,button,1342242816

[CLS:CConfigDlg]
Type=0
HeaderFile=ConfigDlg.h
ImplementationFile=ConfigDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_NONMAGIC

[DLG:IDD_MODDLG]
Type=1
Class=CModDlg
ControlCount=17
Control1=IDC_COMBO1,combobox,1344340227
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308352
Control4=IDC_VAL1,edit,1350631552
Control5=IDC_HELP1,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_VAL2,edit,1350631552
Control8=IDC_HELP2,button,1342242816
Control9=IDC_STATIC,static,1342308352
Control10=IDC_VAL3,edit,1350631552
Control11=IDC_STATIC,button,1342177287
Control12=IDC_PREVIEW,edit,1350633600
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,static,1342308352
Control15=IDOK,button,1342242816
Control16=IDCANCEL,button,1342242816
Control17=IDC_STATIC,static,1342308352

[CLS:CModDlg]
Type=0
HeaderFile=ModDlg.h
ImplementationFile=ModDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_PREVIEW

[DLG:IDD_ASSISTDLG]
Type=1
Class=CAssistDlg
ControlCount=3
Control1=IDC_LIST1,SysListView32,1350631429
Control2=IDOK,button,1476460544
Control3=IDCANCEL,button,1342242816

[CLS:CAssistDlg]
Type=0
HeaderFile=AssistDlg.h
ImplementationFile=AssistDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_LIST1

[DLG:IDD_SKILLTYPEDLG]
Type=1
Class=CSkillTypeDlg
ControlCount=7
Control1=IDC_SKILLTYPE,button,1342308361
Control2=IDC_RADIO2,button,1342177289
Control3=IDC_RADIO3,button,1342177289
Control4=IDC_RADIO4,button,1342177289
Control5=IDC_RADIO5,button,1342177289
Control6=IDC_RADIO6,button,1342177289
Control7=IDC_RADIO7,button,1342177289

[CLS:CSkillTypeDlg]
Type=0
HeaderFile=SkillTypeDlg.h
ImplementationFile=SkillTypeDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

