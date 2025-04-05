//lcd
#define LCD_D 0xf<<20
#define RS 1<<17
#define RW 1<<18
#define EN 1<<19
typedef unsigned char U8;
void LCD_INIT(void);
void LCD_CMD(U8 );
void LCD_DATA(U8 );
void LCD_STR(U8* );
void LCD_integer(int );
void LCD_INIT(void)
{
IODIR1=LCD_D|RS|RW|EN;//SELECTING OUTPUT DIRATION
LCD_CMD(0X01);//CLEAR THE DISPLAY
LCD_CMD(0X02);//CURSOR RETURN TO THE HOME POS
LCD_CMD(0X0C);//DISPLAY ON CURSOR OFF
LCD_CMD(0X28);//4 BIT INTERFACING MODE
LCD_CMD(0X80);//STARTING ADDR OF DDRAM
}
void LCD_CMD(U8 C)
{
IOCLR1=LCD_D;
IOSET1=(C&0xf0)<<16;
IOCLR1=RS;//SELECT CMD REGISTER
IOCLR1=RW;//SELECT WRITE MODE
IOSET1=EN;
millisec_delay(2);
IOCLR1=EN;
IOCLR1=LCD_D;
IOSET1=(C&0x0f)<<20;
IOCLR1=RS;//SELECT CMD REGISTER
IOCLR1=RW;//SELECT WRITE MODE
IOSET1=EN;
millisec_delay(2);
IOCLR1=EN;
}
void LCD_DATA(U8 D)
{
IOCLR1=LCD_D;
IOSET1=(D&0xf0)<<16;
IOSET1=RS;//SELECT DATA REGISTER
IOCLR1=RW;
IOSET1=EN;
millisec_delay(2);
IOCLR1=EN;
IOCLR1=LCD_D;
IOSET1=(D&0x0f)<<20;
IOSET1=RS;//SELECT DATA REGISTER
IOCLR1=RW;
IOSET1=EN;
millisec_delay(2);
IOCLR1=EN;
}
void LCD_STR(U8 *s)
{
while(*s)
{
LCD_DATA(*s++);
}
}
void LCD_integer(int n)
{
char arr[20];
int i=0;
if(n==0)
{
LCD_DATA('0');
}
if(n<0)
{
LCD_DATA('-');
}
while(n>0)
{
arr[i++]=n%10;
n=n/10;
}
for(--i;i>=0;i--)
{
LCD_DATA(arr[i]+48);
}
}
/*delay
onesec_delay(int sec)
{
T0PR=15000000-1;
T0TCR=0X01;
while(T0TC<sec);
T0TCR=0X03;
T0TCR=0X00;
}
mirosec_delay(int sec)
{
T0PR=15-1;
T0TCR=0X01;
while(T0TC<sec);
T0TCR=0X03;
T0TCR=0X00;
}
millisec_delay(int sec)
{
T0PR=15000-1;
T0TCR=0X01;
while(T0TC<sec);
T0TCR=0X03;
T0TCR=0X00;
/*main
#include<lpc21xx.h>
#include<string.h>
#include"delay.h"
#include"LCD.h"
#include"uart.h"
#define sw1 1<<15
#define sw2 1<<16
typedef unsigned char U8;
void swt1(void)__irq;
void swt2(void)__irq ;
int product_price=0,total_price=0;
int main()
{
char s[1024];
char book[10]="03115952";
char pen[10]="03054101";
int l=0,t=0,k1,k2;
UART0_INIT();
LCD_INIT();
UART0_STR("STATIONERY SHOP");
LCD_STR("STATIONERY SHOP");
//PINSEL1 |=0x00000001;//PIN 16
PINSEL0 |=0XA0000000;//PIN 15,16
VICIntSelect=0;
VICVectCntl0=(0x20)|15;
VICVectAddr0=(int)swt1;
VICVectCntl1=(0x20)|16;
VICVectAddr1=(int)swt2;
EXTMODE=0X00;
EXTPOLAR=0X00;
VICIntEnable=(1<<15)|(1<<16);
while(1)
{
k1=1;k2=1;
for(l=0;l<9;l++)
s[l]='\0';
for(l=0,t=0;l<9;l++)
{
if(t==0)
{
UART0_RX(); t++;
}
s[l]=UART0_RX();
UART0_TX(s[l]);
}
s[--l]='\0';
millisec_delay(200);
k1=strcmp(book,s);
k2=strcmp(pen,s);
if(k1==0)
{
LCD_CMD(0x01);
LCD_CMD(0x80);
LCD_STR(" BOOK:$10 ");
UART0_STR(" BOOK:$10 ");
product_price=10;
millisec_delay(10);
}
if(k2==0)
{
LCD_CMD(0x01);
LCD_CMD(0x80);
LCD_STR(" PEN:$5 ");
UART0_STR(" PEN:$5 ");
product_price=5;
millisec_delay(10);
}
total_price=product_price+total_price;
UART0_STR((U8*)total_price+48);
LCD_CMD(0xc0);
LCD_STR("$");
LCD_integer(total_price);
}
}
void swt1(void)__irq
{
// EXTINT=0x02;
LCD_CMD(0x01);
LCD_CMD(0x80);
total_price=total_price-product_price;
if(product_price<=5)
{
UART0_STR("Pen removed");
LCD_STR("Pen removed");
}
else
{
UART0_STR("Book removed");
LCD_STR("Book removed");
}
UART0_STR((U8*)total_price+48);
LCD_CMD(0xc0);
LCD_STR("$");
if(total_price<=0)
{
LCD_integer(0);
}
else
{
LCD_integer(total_price);
}
millisec_delay(500);
//while(((IOPIN0>>15)&1)==0);
// VICVectAddr=0;
EXTINT=0x02;
VICVectAddr=0;
}
void swt2(void)__irq
{
// EXTINT=0x04;
LCD_CMD(0x01);
LCD_CMD(0x80);
LCD_STR("THANKYOU");
UART0_STR("Thankyou");
LCD_CMD(0xc0);
LCD_STR("TOTAL BILL:$");
UART0_STR("TOTAL BILL:$");
if(total_price<=0)
{
LCD_integer(0);
}
else
{
LCD_integer(total_price);
}
millisec_delay(500);
//while(((IOPIN0>>16)&1)==0);
// VICVectAddr=0;
EXTINT=0x04;
VICVectAddr=0;
}
/*uart buad rate
typedef unsigned char U8;
void UART0_INIT(void);
char UART0_RX(void);
void UART0_TX(U8 );
void UART0_STR(U8 *);
void UART0_INIT(void)
{
PINSEL0=0X05;
U0LCR=0X83;
U0DLL=97;
U0LCR=0X03;
}
char UART0_RX(void)
{
while((U0LSR&1)==0);
return U0RBR;
}
void UART0_TX(U8 C)
{
while(((U0LSR)&(1<<5))==0);
U0THR=C;
}
void UART0_STR(U8 *s)
{
while(*s)
{
UART0_TX(*s++);
}
*/