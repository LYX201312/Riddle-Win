#include<graphics.h>
#include<ege/sys_edit.h>
#include<Windows.h>
#include<string>
#include<iostream>

using namespace std;

char name[8] = "";

//变量区
int cx = GetSystemMetrics(SM_CXSCREEN);
int cy = GetSystemMetrics(SM_CYSCREEN);
wchar_t path[MAX_PATH];
wstring que[500];
wstring ans[500];
wstring why[500];



BOOL TEMP1, TEMP2;
int count1 = 0;
sys_edit edit;

int ReadQuestion() {
	cleardevice();
	settextcolor(EGERGB(0, 0, 125));
	setfont(50, 0, "方正姚体");
	settextjustify(LEFT_TEXT, TOP_TEXT);
	count1 = 0;
	wchar_t wen[500];
	wchar_t da[500] ;
	wchar_t wei[500];
//	cout << "1";
	TEMP1 = TRUE;
	while (TEMP1) {
		st:
		wstring TEMP=L"Unit"+to_wstring(count1);
		GetPrivateProfileStringW(TEMP.c_str(), L"Question", L"ERROR_STR", wen, sizeof(wen), path);
		GetPrivateProfileStringW(TEMP.c_str(), L"Answer", L"ERROR_STR", da, sizeof(da), path);
		GetPrivateProfileStringW(TEMP.c_str(), L"Why", L"无", wei, sizeof(wei), path);
//		cout<<"Dump:"<<count1<<" "<<TEMP4<<" "<<TEMP5<<endl;
//		Sleep(10);
		if (wen[0] == 'E') {
			goto error;
		} else {
			if (da[0] == 'E') {
				goto error;
			} else {
				que[count1] = wen;
				ans[count1] = da;
				why[count1] = wei;
				count1++;
				goto st;
			}
		}
		error:
		break;
	}
	outtextxy(0, 0, "==============");
	outtextxy(0, 50, "已发现");
	outtextxy(150, 50, (to_string(count1) += "题").c_str());
	outtextxy(0, 100, "是否重新搜索?");
	outtextxy(0, 150, "Y->是 N->否");
	outtextxy(0, 200, "==============");
	while (1) {
		int ch = getch();
		if (ch == 'Y' || ch == 'y') {
			return 1;
		}
		if (ch == 'N' || ch == 'n') {
			if(count1!=0){
				return 0;
			}
		}
	}
	return 1;
}

BOOL input_where() {
	cleardevice();
	settextjustify(CENTER_TEXT,CENTER_LINE);
	settextcolor(EGERGB(0, 0, 125));
	setfont(100, 0, "方正姚体");
	outtextxy(cx/2,cy/2,"请在选择框中选择题库文件");
	delay_fps(60);
	OPENFILENAMEW opfn;
	ZeroMemory(&opfn,sizeof(opfn));
	opfn.lStructSize=sizeof(opfn);
	opfn.hwndOwner=GetForegroundWindow();
	opfn.lpstrFilter=L"Riddle题库文件\0*.riddle\0所有文件\0*.*\0\0";
	opfn.nFilterIndex=1;
	path[0]='\0';
	opfn.lpstrFile=path;
	opfn.nMaxFile=sizeof(path);
	opfn.Flags=OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
	if(GetOpenFileNameW(&opfn)){
//		path=opfn.lpstrFile;
		return true;
	}else{
		return false;
	}
	
}

void single() {
	a:
	if(!input_where()){
		MessageBox(GetForegroundWindow(),"您选择了取消操作!","提示",MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (ReadQuestion() == 0) {
		cleardevice();
		edit.destroy();
		delay_fps(60);
		while (1) {
			st:
			int res = rand() % count1;
			settextjustify(CENTER_TEXT, TOP_TEXT);
			xyprintf(cx / 2, 0, "答题模式");
			settextjustify(LEFT_TEXT, TOP_TEXT);
			outtextxy(0, 50, "题目id:");
			outtextxy(175, 50, to_string(res).c_str());
			outtextxy(0, 100, "谜面:");
			outtextxy(125, 100, que[res].c_str());
			outtextxy(0, 150, "输入答案>");
			sys_edit edt;
			edt.create();
			edt.size(50, 50);
			edt.move(200, 150);
			edt.visible(true);
			edt.setfont(35, 0, "");
			edt.setmaxlen(1);
			outtextxy(250, 150, "按Enter提交,按Esc退出");
			delay_fps(60);
			BOOL B = TRUE;
			while (B == TRUE) {
				int gg=getch();
				if(gg==13){
					wchar_t wchbuffer[2]=L"";
					edt.gettext(2,wchbuffer);
					wstring wstr=wchbuffer;
					if(wstr==ans[res]){
						MessageBoxA(0,"答对了","Riddle - Win",MB_OK|MB_ICONINFORMATION|MB_SETFOREGROUND);
						string str = (string)"./" + name + ".ini";
						cout<<str<<endl;
						int jy=GetPrivateProfileInt("User","Exper",0,str.c_str());
						cout<<jy<<endl;
						jy++;
						cout<<jy<<endl;
						WritePrivateProfileString("User","Exper",to_string(jy).c_str(),str.c_str());
						int lv=jy/90;
						WritePrivateProfileString("User","Level",to_string(lv).c_str(),str.c_str());
						break;
					}else{
						int k = MessageBoxA(GetForegroundWindow(),"答错了\n按重试重新答题\n按取消显示答案并进入下一题","Riddle - Win",MB_RETRYCANCEL|MB_ICONHAND);
						if(k==IDCANCEL){
							wstring w=(wstring)L"您选择了取消答题\n"+L"此题答案为:"+ans[res]+L"\n"+L"答案解析:"+why[res];
							MessageBoxW(GetForegroundWindow(),w.c_str(),L"Riddle -Win",MB_OK|MB_ICONINFORMATION|MB_SETFOREGROUND);
							break;
						}
					}
				}else{
					if(gg==27){
						if(MessageBox(GetForegroundWindow(),"您是否确定退出?","Riddle - Win",MB_YESNO|MB_ICONQUESTION)==6)return;
					}
				}
				
			}
//			cout<<
			cleardevice();
		}
	} else {
		cleardevice();
		delay_fps(60);
		goto a;
	}
	
}




