#include "StdAfx.h"
#include "element.h"
element::element()
{
	int qd;
	int zd;
	//int unit;//1,2,3,4,5,6�ֱ����x��y��z��˼���xy��xz��yz�������
	float xs;
	float ys;
	float zs;
	float xe;
	float ye;
	float ze;
}
void element::operator=(element &n)
{
	qd=n.qd;
	zd=n.zd;
	xs=n.xs;
	ys=n.ys;
	zs=n.zs;
	xe=n.xe;
	ye=n.ye;
	ze=n.ze;
}
