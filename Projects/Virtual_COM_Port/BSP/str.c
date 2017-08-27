/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： str.c
*	@Brief   	： 
*	@Author  	： Blade@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2016-05
*	@Description： 
*	@History	： 
*
*	Rev1.0 
*		Date：2016-05-16
*		Author：Blade
*		Modification：
*
*--------------------Copyright (C) oarap.org 2016------------------------------
*----------------------All rights reserved-------------------------------------
*
********************          一名工匠的遗憾与骄傲          ********************/
#include "Sys_Conf.h"

//对比字符串str1和str2
//*str1:字符串1指针
//*str2:字符串2指针
//返回值:0，相等;1，不相等;
/////////新增，自动按照第一个字符串的长度来对比
u8 blade_strcmp(u8 *str1,u8 *str2)
{
  u8 i=blade_strlen(str1);
	while(i>0)
	{
		if(*str1!=*str2)return 1;//不相等
		if(*str1=='\0')break;//对比完成了.
		str1++;
		str2++;
		i--;
	}
	return 0;//两个字符串相等
}
//把str1的内容copy到str2
//*str1:字符串1指针
//*str2:字符串2指针			   
void blade_strcopy(u8*str1,u8 *str2)
{
	while(1)
	{										   
		*str2=*str1;	//拷贝
		if(*str1=='\0')break;//拷贝完成了.
		str1++;
		str2++;
	}
}
//得到字符串的长度(字节)
//*str:字符串指针
//返回值:字符串的长度		   
u8 blade_strlen(u8*str)
{
	u8 len=0;
	while(1)
	{							 
		if(*str=='\0')break;
		len++;
		str++;
	}
	return len;
}