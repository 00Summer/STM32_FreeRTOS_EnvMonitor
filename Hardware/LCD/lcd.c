#include "lcd.h"
#include "stdlib.h"
#include "lcdfont.h"
#include "usart.h"	 
#include "delay.h"
  
//¹ÜÀíLCDÖØÒª²ÎÊı
//Ä¬ÈÏÎªÊúÆÁ
_lcd_dev lcddev;
	 
//Ğ´¼Ä´æÆ÷º¯Êı
//regval:¼Ä´æÆ÷Öµ
void LCD_WR_REG(u16 reg)
{   
	LCD->LCD_REG=reg;//Ğ´ÈëÒªĞ´µÄ¼Ä´æÆ÷ĞòºÅ	 
}
//Ğ´LCDÊı¾İ
//data:ÒªĞ´ÈëµÄÖµ
void LCD_WR_DATA(u16 data)
{	 
	LCD->LCD_RAM=data;		 
}
//¶ÁLCDÊı¾İ
//·µ»ØÖµ:¶Áµ½µÄÖµ
u16 LCD_RD_DATA(void)
{
	vu16 ram;			//·ÀÖ¹±»ÓÅ»¯
	ram=LCD->LCD_RAM;	
	return ram;	 
}


/******************************************************************************
      º¯ÊıËµÃ÷£ºÉèÖÃÆğÊ¼ºÍ½áÊøµØÖ·
      Èë¿ÚÊı¾İ£ºx1,x2 ÉèÖÃÁĞµÄÆğÊ¼ºÍ½áÊøµØÖ·
                y1,y2 ÉèÖÃĞĞµÄÆğÊ¼ºÍ½áÊøµØÖ·
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
		LCD_WR_REG(0x2a);//ÁĞµØÖ·ÉèÖÃ
		LCD_WR_DATA(x1>>8);
		LCD_WR_DATA(x1&0xff);
		LCD_WR_DATA(x2>>8);
		LCD_WR_DATA(x2&0xff);
		LCD_WR_REG(0x2b);//ĞĞµØÖ·ÉèÖÃ
		LCD_WR_DATA(y1>>8);
		LCD_WR_DATA(y1&0xff);
		LCD_WR_DATA(y2>>8);
		LCD_WR_DATA(y2&0xff);
		LCD_WR_REG(0x2c);//´¢´æÆ÷Ğ´
}

/******************************************************************************
      º¯ÊıËµÃ÷£ºÉèÖÃ¹â±êÎ»ÖÃ
      Èë¿ÚÊı¾İ£ºx,y ¹â±êÎ»ÖÃ
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_SetCursor(u16 x,u16 y)
{
	LCD_WR_REG(0x2a);//ÁĞµØÖ·ÉèÖÃ
	LCD_WR_DATA(x>>8);
	LCD_WR_DATA(x&0xff);
	LCD_WR_REG(0x2b);//ĞĞµØÖ·ÉèÖÃ
	LCD_WR_DATA(y>>8);
	LCD_WR_DATA(y&0xff);
}

//LCD¿ªÆôÏÔÊ¾
void LCD_DisplayOn(void)
{
	LCD_WR_REG(0X29);	//¿ªÆôÏÔÊ¾
}	 
//LCD¹Ø±ÕÏÔÊ¾
void LCD_DisplayOff(void)
{
	LCD_WR_REG(0X28);	//¹Ø±ÕÏÔÊ¾
}  


//¶ÁÈ¡¸öÄ³µãµÄÑÕÉ«Öµ	 
//x,y:×ø±ê
//·µ»ØÖµ:´ËµãµÄÑÕÉ«
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 rgb;
	LCD_SetCursor(x,y);
	LCD_WR_REG(0X2E);
	rgb=LCD_RD_DATA();//dummy Read
	rgb=LCD_RD_DATA();//Êµ¼Ê×ø±êÑÕÉ«
	return rgb;
}


void LCD_Clear(u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(0,0,lcddev.width-1,lcddev.height-1);//ÉèÖÃÏÔÊ¾·¶Î§
	for(i=0;i<lcddev.width;i++)
	{													   	 	
		for(j=0;j<lcddev.height;j++)
		{
			LCD_WR_DATA(color);
		}
	} 					  	    
}

/******************************************************************************
      º¯ÊıËµÃ÷£ºÔÚÖ¸¶¨ÇøÓòÌî³äÑÕÉ«
      Èë¿ÚÊı¾İ£ºxsta,ysta   ÆğÊ¼×ø±ê
                xend,yend   ÖÕÖ¹×ø±ê
								color       ÒªÌî³äµÄÑÕÉ«
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//ÉèÖÃÏÔÊ¾·¶Î§
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA(color);
		}
	} 					  	    
}

/******************************************************************************
      º¯ÊıËµÃ÷£ºÔÚÖ¸¶¨Î»ÖÃ»­µã
      Èë¿ÚÊı¾İ£ºx,y »­µã×ø±ê
                color µãµÄÑÕÉ«
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//ÉèÖÃ¹â±êÎ»ÖÃ 
	LCD_WR_DATA(color);
} 


/******************************************************************************
      º¯ÊıËµÃ÷£º»­Ïß
      Èë¿ÚÊı¾İ£ºx1,y1   ÆğÊ¼×ø±ê
                x2,y2   ÖÕÖ¹×ø±ê
                color   ÏßµÄÑÕÉ«
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //¼ÆËã×ø±êÔöÁ¿ 
	delta_y=y2-y1;
	uRow=x1;//»­ÏßÆğµã×ø±ê
	uCol=y1;
	if(delta_x>0)incx=1; //ÉèÖÃµ¥²½·½Ïò 
	else if (delta_x==0)incx=0;//´¹Ö±Ïß 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//Ë®Æ½Ïß 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //Ñ¡È¡»ù±¾ÔöÁ¿×ø±êÖá 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//»­µã
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/******************************************************************************
      º¯ÊıËµÃ÷£º»­¾ØĞÎ
      Èë¿ÚÊı¾İ£ºx1,y1   ÆğÊ¼×ø±ê
                x2,y2   ÖÕÖ¹×ø±ê
                color   ¾ØĞÎµÄÑÕÉ«
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      º¯ÊıËµÃ÷£º»­Ô²
      Èë¿ÚÊı¾İ£ºx0,y0   Ô²ĞÄ×ø±ê
                r       °ë¾¶
                color   Ô²µÄÑÕÉ«
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//ÅĞ¶ÏÒª»­µÄµãÊÇ·ñ¹ıÔ¶
		{
			b--;
		}
	}
}

/******************************************************************************
      º¯ÊıËµÃ÷£ºÏÔÊ¾ºº×Ö´®
      Èë¿ÚÊı¾İ£ºx,yÏÔÊ¾×ø±ê
                *s ÒªÏÔÊ¾µÄºº×Ö´®
                fc ×ÖµÄÑÕÉ«
                bc ×ÖµÄ±³¾°É«
                sizey ×ÖºÅ ¿ÉÑ¡ 16 24 32
                mode:  0·Çµş¼ÓÄ£Ê½  1µş¼ÓÄ£Ê½
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	while(*s!=0)
	{
		if(sizey==12) LCD_ShowChinese12x12(x,y,s,fc,bc,sizey,mode);
		else if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}

/******************************************************************************
      º¯ÊıËµÃ÷£ºÏÔÊ¾µ¥¸ö12x12ºº×Ö
      Èë¿ÚÊı¾İ£ºx,yÏÔÊ¾×ø±ê
                *s ÒªÏÔÊ¾µÄºº×Ö
                fc ×ÖµÄÑÕÉ«
                bc ×ÖµÄ±³¾°É«
                sizey ×ÖºÅ
                mode:  0·Çµş¼ÓÄ£Ê½  1µş¼ÓÄ£Ê½
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_ShowChinese12x12(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//ºº×ÖÊıÄ¿
	u16 TypefaceNum;//Ò»¸ö×Ö·ûËùÕ¼×Ö½Ú´óĞ¡
	u16 x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	                         
	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//Í³¼Æºº×ÖÊıÄ¿
	for(k=0;k<HZnum;k++) 
	{
		if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//·Çµş¼Ó·½Ê½
					{
						if(tfont12[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//µş¼Ó·½Ê½
					{
						if(tfont12[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//»­Ò»¸öµã
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //²éÕÒµ½¶ÔÓ¦µãÕó×Ö¿âÁ¢¼´ÍË³ö£¬·ÀÖ¹¶à¸öºº×ÖÖØ¸´È¡Ä£´øÀ´Ó°Ïì
	}
} 

/******************************************************************************
      º¯ÊıËµÃ÷£ºÏÔÊ¾µ¥¸ö16x16ºº×Ö
      Èë¿ÚÊı¾İ£ºx,yÏÔÊ¾×ø±ê
                *s ÒªÏÔÊ¾µÄºº×Ö
                fc ×ÖµÄÑÕÉ«
                bc ×ÖµÄ±³¾°É«
                sizey ×ÖºÅ
                mode:  0·Çµş¼ÓÄ£Ê½  1µş¼ÓÄ£Ê½
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//ºº×ÖÊıÄ¿
	u16 TypefaceNum;//Ò»¸ö×Ö·ûËùÕ¼×Ö½Ú´óĞ¡
	u16 x0=x;
  TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//Í³¼Æºº×ÖÊıÄ¿
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//·Çµş¼Ó·½Ê½
					{
						if(tfont16[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//µş¼Ó·½Ê½
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//»­Ò»¸öµã
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //²éÕÒµ½¶ÔÓ¦µãÕó×Ö¿âÁ¢¼´ÍË³ö£¬·ÀÖ¹¶à¸öºº×ÖÖØ¸´È¡Ä£´øÀ´Ó°Ïì
	}
} 


/******************************************************************************
      º¯ÊıËµÃ÷£ºÏÔÊ¾µ¥¸ö24x24ºº×Ö
      Èë¿ÚÊı¾İ£ºx,yÏÔÊ¾×ø±ê
                *s ÒªÏÔÊ¾µÄºº×Ö
                fc ×ÖµÄÑÕÉ«
                bc ×ÖµÄ±³¾°É«
                sizey ×ÖºÅ
                mode:  0·Çµş¼ÓÄ£Ê½  1µş¼ÓÄ£Ê½
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//ºº×ÖÊıÄ¿
	u16 TypefaceNum;//Ò»¸ö×Ö·ûËùÕ¼×Ö½Ú´óĞ¡
	u16 x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//Í³¼Æºº×ÖÊıÄ¿
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//·Çµş¼Ó·½Ê½
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//µş¼Ó·½Ê½
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//»­Ò»¸öµã
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //²éÕÒµ½¶ÔÓ¦µãÕó×Ö¿âÁ¢¼´ÍË³ö£¬·ÀÖ¹¶à¸öºº×ÖÖØ¸´È¡Ä£´øÀ´Ó°Ïì
	}
} 

/******************************************************************************
      º¯ÊıËµÃ÷£ºÏÔÊ¾µ¥¸ö32x32ºº×Ö
      Èë¿ÚÊı¾İ£ºx,yÏÔÊ¾×ø±ê
                *s ÒªÏÔÊ¾µÄºº×Ö
                fc ×ÖµÄÑÕÉ«
                bc ×ÖµÄ±³¾°É«
                sizey ×ÖºÅ
                mode:  0·Çµş¼ÓÄ£Ê½  1µş¼ÓÄ£Ê½
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//ºº×ÖÊıÄ¿
	u16 TypefaceNum;//Ò»¸ö×Ö·ûËùÕ¼×Ö½Ú´óĞ¡
	u16 x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//Í³¼Æºº×ÖÊıÄ¿
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//·Çµş¼Ó·½Ê½
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//µş¼Ó·½Ê½
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//»­Ò»¸öµã
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //²éÕÒµ½¶ÔÓ¦µãÕó×Ö¿âÁ¢¼´ÍË³ö£¬·ÀÖ¹¶à¸öºº×ÖÖØ¸´È¡Ä£´øÀ´Ó°Ïì
	}
}


/******************************************************************************
      º¯ÊıËµÃ÷£ºÏÔÊ¾µ¥¸ö×Ö·û
      Èë¿ÚÊı¾İ£ºx,yÏÔÊ¾×ø±ê
                num ÒªÏÔÊ¾µÄ×Ö·û
                fc ×ÖµÄÑÕÉ«
                bc ×ÖµÄ±³¾°É«
                sizey ×ÖºÅ
                mode:  0·Çµş¼ÓÄ£Ê½  1µş¼ÓÄ£Ê½
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
/******************************************************************************
      º¯ÊıËµÃ÷£ºÏÔÊ¾µ¥¸ö×Ö·û
      Èë¿ÚÊı¾İ£ºx,yÏÔÊ¾×ø±ê
                num ÒªÏÔÊ¾µÄ×Ö·û
                fc ×ÖµÄÑÕÉ«
                bc ×ÖµÄ±³¾°É«
                sizey ×ÖºÅ
                mode:  0·Çµş¼ÓÄ£Ê½  1µş¼ÓÄ£Ê½
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp,sizex,t,m=0;
	u16 i,TypefaceNum;//Ò»¸ö×Ö·ûËùÕ¼×Ö½Ú´óĞ¡
	u16 x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //µÃµ½Æ«ÒÆºóµÄÖµ
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //ÉèÖÃ¹â±êÎ»ÖÃ 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];		       //µ÷ÓÃ6x12×ÖÌå
		else if(sizey==16)temp=ascii_1608[num][i];		 //µ÷ÓÃ8x16×ÖÌå
		else if(sizey==24)temp=ascii_2412[num][i];		 //µ÷ÓÃ12x24×ÖÌå
		else if(sizey==32)temp=ascii_3216[num][i];		 //µ÷ÓÃ16x32×ÖÌå
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//·Çµş¼ÓÄ£Ê½
			{
				if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//µş¼ÓÄ£Ê½
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//»­Ò»¸öµã
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}


/******************************************************************************
      º¯ÊıËµÃ÷£ºÏÔÊ¾×Ö·û´®
      Èë¿ÚÊı¾İ£ºx,yÏÔÊ¾×ø±ê
                *p ÒªÏÔÊ¾µÄ×Ö·û´®
                fc ×ÖµÄÑÕÉ«
                bc ×ÖµÄ±³¾°É«
                sizey ×ÖºÅ
                mode:  0·Çµş¼ÓÄ£Ê½  1µş¼ÓÄ£Ê½
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


/******************************************************************************
      º¯ÊıËµÃ÷£ºÏÔÊ¾Êı×Ö
      Èë¿ÚÊı¾İ£ºmµ×Êı£¬nÖ¸Êı
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}


/******************************************************************************
      º¯ÊıËµÃ÷£ºÏÔÊ¾ÕûÊı±äÁ¿
      Èë¿ÚÊı¾İ£ºx,yÏÔÊ¾×ø±ê
                num ÒªÏÔÊ¾ÕûÊı±äÁ¿
                len ÒªÏÔÊ¾µÄÎ»Êı
                fc ×ÖµÄÑÕÉ«
                bc ×ÖµÄ±³¾°É«
                sizey ×ÖºÅ
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp;
	u8 enshow=0;
	u8 sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
} 


/******************************************************************************
      º¯ÊıËµÃ÷£ºÏÔÊ¾Á½Î»Ğ¡Êı±äÁ¿
      Èë¿ÚÊı¾İ£ºx,yÏÔÊ¾×ø±ê
                num ÒªÏÔÊ¾Ğ¡Êı±äÁ¿
                len ÒªÏÔÊ¾µÄÎ»Êı
                fc ×ÖµÄÑÕÉ«
                bc ×ÖµÄ±³¾°É«
                sizey ×ÖºÅ
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp,sizex;
	u16 num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}


/******************************************************************************
      º¯ÊıËµÃ÷£ºÏÔÊ¾Í¼Æ¬
      Èë¿ÚÊı¾İ£ºx,yÆğµã×ø±ê
                length Í¼Æ¬³¤¶È
                width  Í¼Æ¬¿í¶È
                pic[]  Í¼Æ¬Êı×é    
      ·µ»ØÖµ£º  ÎŞ
******************************************************************************/
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[])
{
	u8 picH,picL;
	u16 i,j;
	u32 k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			picH=pic[k*2];
			picL=pic[k*2+1];
			LCD_WR_DATA(picH<<8|picL);
			k++;
		}
	}			
}


void Set_Dir(u8 dir)
{
	if((dir>>4)%4)
	{
		lcddev.width=480;
		lcddev.height=320;
	}else
	{
		lcddev.width=320;
		lcddev.height=480;
	}
}



//³õÊ¼»¯lcd
void LCD_Init(void)
{                   
    GPIO_InitTypeDef GPIO_InitStructure;
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
    FSMC_NORSRAMTimingInitTypeDef  writeTiming;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE); //Ê¹ÄÜFSMCÊ±ÖÓ
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG,ENABLE);//Ê¹ÄÜPORTB,D,E,GÒÔ¼°AFIO¸´ÓÃ¹¦ÄÜÊ±ÖÓ

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                //PB0 ÍÆÍìÊä³ö ±³¹â
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //ÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //PORTD¸´ÓÃÍÆÍìÊä³ö  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //¸´ÓÃÍÆÍìÊä³ö   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 
     
    //PORTE¸´ÓÃÍÆÍìÊä³ö  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //¸´ÓÃÍÆÍìÊä³ö   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);                                                     

    //PORTG12¸´ÓÃÍÆÍìÊä³ö A0    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //¸´ÓÃÍÆÍìÊä³ö   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure); 
		
		// ===================================================
    // è¡¥å…¨çš„ç»ˆææ‹¼å›¾ï¼šåˆå§‹åŒ– PG15 å¹¶æ‰§è¡Œ LCD ç¡¬ä»¶å¤ä½
    // ===================================================
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;               // é€‰æ‹© PG15 (LCD_RST)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         // æ¨æŒ½è¾“å‡º
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure); 
    
    // æ‰§è¡Œç‰©ç†é‡å¯è„‰å†²
    GPIO_ResetBits(GPIOG, GPIO_Pin_15); // ç‰©ç†æ‹‰ä½ï¼ŒæŒ‰ä½å¤ä½é”®
    delay_ms(100);                      // ä¿æŒ 100ms
    GPIO_SetBits(GPIOG, GPIO_Pin_15);   // ç‰©ç†æ‹‰é«˜ï¼Œæ¾å¼€å¤ä½é”®
    delay_ms(100);                      // ç­‰å¾…èŠ¯ç‰‡è„‘å­æ¸…é†’
    // ===================================================

    readWriteTiming.FSMC_AddressSetupTime = 0x01;    //µØÖ·½¨Á¢Ê±¼ä£¨ADDSET£©Îª2¸öHCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;   //µØÖ·±£³ÖÊ±¼ä£¨ADDHLD£©Ä£Ê½AÎ´ÓÃµ½    
    readWriteTiming.FSMC_DataSetupTime = 0x0f;         // Êı¾İ±£´æÊ±¼äÎª16¸öHCLK
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;   //Ä£Ê½A 
    
    writeTiming.FSMC_AddressSetupTime = 0x00;    //µØÖ·½¨Á¢Ê±¼ä£¨ADDSET£©Îª1¸öHCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;   //µØÖ·±£³ÖÊ±¼ä     
    writeTiming.FSMC_DataSetupTime = 0x03;         ////Êı¾İ±£´æÊ±¼äÎª4¸öHCLK  
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;   //Ä£Ê½A 

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM; 
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; 
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; 
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  

    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // Ê¹ÄÜBANK1 
 
    delay_ms(50);                   // delay 50 ms 
    Set_Dir(DFT_SCAN_DIR);
    LCD_WR_REG(0XD3);                  
    lcddev.id=LCD_RD_DATA();    //dummy read    
    lcddev.id=LCD_RD_DATA();    //¶Áµ½0X00
    lcddev.id=LCD_RD_DATA();    //¶ÁÈ¡93                                     
    lcddev.id<<=8;
    lcddev.id|=LCD_RD_DATA();   //¶ÁÈ¡41    
    
    // ==========================================
    // ÖÕ¼«ĞŞ¸´µã£º¸ù¾İÕæÊµµÄ ID Æ¥Åä¶ÔÓ¦µÄ³õÊ¼»¯ÃÜÂë
    // ==========================================
    if(lcddev.id == 0x9488)
    {
        // ILI9488 ×¨Êô³õÊ¼»¯ĞòÁĞ
        LCD_WR_REG(0XF7); LCD_WR_DATA(0xA9); LCD_WR_DATA(0x51); LCD_WR_DATA(0x2C); LCD_WR_DATA(0x82);
        LCD_WR_REG(0xC0); LCD_WR_DATA(0x11); LCD_WR_DATA(0x09);
        LCD_WR_REG(0xC1); LCD_WR_DATA(0x41);
        LCD_WR_REG(0XC5); LCD_WR_DATA(0x00); LCD_WR_DATA(0x0A); LCD_WR_DATA(0x80);
        LCD_WR_REG(0xB1); LCD_WR_DATA(0xB0); LCD_WR_DATA(0x11);
        LCD_WR_REG(0xB4); LCD_WR_DATA(0x02);
        LCD_WR_REG(0xB6); LCD_WR_DATA(0x02); LCD_WR_DATA(0x22);
        LCD_WR_REG(0xB7); LCD_WR_DATA(0xc6);
        LCD_WR_REG(0xBE); LCD_WR_DATA(0x00); LCD_WR_DATA(0x04);
        LCD_WR_REG(0xE9); LCD_WR_DATA(0x00);
        
        LCD_WR_REG(0x36);
        LCD_WR_DATA(0x08 | DFT_SCAN_DIR); // ÊÊÅäÄãÔ­ÓĞµÄ·½Ïò¿ØÖÆºê
        
        LCD_WR_REG(0x3A);
        LCD_WR_DATA(0x55); // 16Î»RGB¸ñÊ½
        
        // Ù¤ÂíĞ£×¼
        LCD_WR_REG(0xE0);
        LCD_WR_DATA(0x00); LCD_WR_DATA(0x07); LCD_WR_DATA(0x10);
        LCD_WR_DATA(0x09); LCD_WR_DATA(0x17); LCD_WR_DATA(0x0B);
        LCD_WR_DATA(0x41); LCD_WR_DATA(0x89); LCD_WR_DATA(0x4B);
        LCD_WR_DATA(0x0A); LCD_WR_DATA(0x0C); LCD_WR_DATA(0x0E);
        LCD_WR_DATA(0x18); LCD_WR_DATA(0x1B); LCD_WR_DATA(0x0F);
        
        LCD_WR_REG(0XE1);
        LCD_WR_DATA(0x00); LCD_WR_DATA(0x17); LCD_WR_DATA(0x1A);
        LCD_WR_DATA(0x04); LCD_WR_DATA(0x0E); LCD_WR_DATA(0x06);
        LCD_WR_DATA(0x2F); LCD_WR_DATA(0x45); LCD_WR_DATA(0x43);
        LCD_WR_DATA(0x02); LCD_WR_DATA(0x0A); LCD_WR_DATA(0x09);
        LCD_WR_DATA(0x32); LCD_WR_DATA(0x36); LCD_WR_DATA(0x0F);
        
        LCD_WR_REG(0x11); // Exit Sleep
        delay_ms(120);    // ±ØĞëÑÓÊ±µÈ´ıÓ²¼şËÕĞÑ
        LCD_WR_REG(0x29); // Display on
    }
    else
    {
        // ¼æÈİÔ­ÓĞµÄ 9341/ÆäËûÆÁÄ»´úÂë£¬ÒÔ·ÀÍòÒ»
        LCD_WR_REG(0XF1);
        LCD_WR_DATA(0x36); LCD_WR_DATA(0x04); LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x3C); LCD_WR_DATA(0x0F); LCD_WR_DATA(0x8F);
        LCD_WR_REG(0XF2);
        LCD_WR_DATA(0x18); LCD_WR_DATA(0xA3); LCD_WR_DATA(0x12);
        LCD_WR_DATA(0x02); LCD_WR_DATA(0xB2); LCD_WR_DATA(0x12);
        LCD_WR_DATA(0xFF); LCD_WR_DATA(0x10); LCD_WR_DATA(0x00);
        LCD_WR_REG(0XF8); LCD_WR_DATA(0x21); LCD_WR_DATA(0x04);
        LCD_WR_REG(0XF9); LCD_WR_DATA(0x00); LCD_WR_DATA(0x08);
        LCD_WR_REG(0xE0);
        LCD_WR_DATA(0x00); LCD_WR_DATA(0x10); LCD_WR_DATA(0x0F);
        LCD_WR_DATA(0x0E); LCD_WR_DATA(0x11); LCD_WR_DATA(0x0A);
        LCD_WR_DATA(0x44); LCD_WR_DATA(0x77); LCD_WR_DATA(0x38);
        LCD_WR_DATA(0x08); LCD_WR_DATA(0x13); LCD_WR_DATA(0x05);
        LCD_WR_DATA(0x0B); LCD_WR_DATA(0x02); LCD_WR_DATA(0x00);
        LCD_WR_REG(0XE1);
        LCD_WR_DATA(0x0F); LCD_WR_DATA(0x3B); LCD_WR_DATA(0x39);
        LCD_WR_DATA(0x0D); LCD_WR_DATA(0x0F); LCD_WR_DATA(0x05);
        LCD_WR_DATA(0x53); LCD_WR_DATA(0x43); LCD_WR_DATA(0x3D);
        LCD_WR_DATA(0x05); LCD_WR_DATA(0x0F); LCD_WR_DATA(0x04);
        LCD_WR_DATA(0x23); LCD_WR_DATA(0x1F); LCD_WR_DATA(0x0F);
        LCD_WR_REG(0x36);
        LCD_WR_DATA(0x08|DFT_SCAN_DIR); 
        LCD_WR_REG(0x3A); LCD_WR_DATA(0x55); 
        LCD_WR_REG(0XB1); LCD_WR_DATA(0xB0); LCD_WR_DATA(0x11); 
        LCD_WR_REG(0XB4); LCD_WR_DATA(0x02);
        LCD_WR_REG(0XB6);
        LCD_WR_DATA(0x00); LCD_WR_DATA(0x42); LCD_WR_DATA(0x3B);
        LCD_WR_REG(0XB7); LCD_WR_DATA(0xC6);
        LCD_WR_REG(0XC0); LCD_WR_DATA(0x05); LCD_WR_DATA(0x05);
        LCD_WR_REG(0XC1); LCD_WR_DATA(0x41); LCD_WR_DATA(0x00);
        LCD_WR_REG(0XC2); LCD_WR_DATA(0x22);
        LCD_WR_REG(0XC5); LCD_WR_DATA(0x00); LCD_WR_DATA(0x31); LCD_WR_DATA(0x80); 
        
        LCD_WR_REG(0x11); //Exit Sleep
        delay_ms(120);
        LCD_WR_REG(0x29); //display on  
    }
    
    LCD_LED=1; // ´ò¿ª±³¹â
}





