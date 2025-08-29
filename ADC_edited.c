///////////*************Analog To Digital Conversion****************//////////////
///////////***************Author: Siddhanta Borah***************//////////////////

/********************************************************************************
                                HEADER FILES
*********************************************************************************/
#include<lpc213x.h>
#define RS  0x00020000                  /* RS  - P1.17                          */
#define RW  0X00040000                  /* R/W - P1.18                          */    
#define EN  0X00080000                  /* E   - P1.19                          */
#define CLR 0X00FE0000

/********************************************************************************
                             VARIABLE DECLARATION                                             
*********************************************************************************/
unsigned int adc_value=0;

/********************************************************************************
* Function    : Delay                                                           *
*                                                                               *
* Description : Function for Delay                                              *
*                                                                               *
* Parameters  : x - Delay time                                                  *
*********************************************************************************/
int Delay(unsigned int x)
{
 x=x*12000;
 while(x!=0)
 {
  x--;
 }
 return 0;
}

/********************************************************************************
* Function    : LCD_Command                                                     *
*                                                                               *
* Description : Function to give command to LCD                                 *
*                                                                               *
* Parameters  : command - command                                               *
*********************************************************************************/
void LCD_Command(char command)
{
 int Temp;				           
 IO1CLR = CLR;                          /* Clearing the port pins               */
 IO1SET = EN;                           /* Enable pin high                      */
 IO1CLR = RS;                           /* RS=0 for command register            */
 IO1CLR = RW;                           /* R/W=0 for write                      */
 Temp   = (command & 0xF0) << 16;       /* Taking the first nibble of command   */
 IO1SET = IO1SET | Temp;                /* Writing it to data line              */
 Delay(2);
 IO1CLR = EN;                           /* Enable pin low to give H-L pulse     */
}

/********************************************************************************
* Function    : LCD_Command1                                                    *
*                                                                               *
* Description : Function to give command to LCD                                 *
*                                                                               *
* Parameters  : command1 - command                                              *
*********************************************************************************/
void LCD_Command1(char command1)
{
 int Temp;
 IO1CLR = CLR;                          /* Clearing the port pins               */
 IO1SET = EN;                           /* Enable pin high                      */
 IO1CLR = RS;                           /* RS=0 for command register            */
 IO1CLR = RW;                           /* R/W=0 for write                      */
 Temp   = (command1 & 0xF0) << 16;      /* Taking the first nibble of command   */
 IO1SET = IO1SET | Temp;                /* Writing it to data line              */
 Delay(2);
 IO1CLR = EN;                           /* Enable pin low to give H-L pulse     */

 IO1CLR = CLR;                          /* Clearing the port pins               */
 IO1SET = EN;                           /* Enable pin high                      */
 IO1CLR = RS;                           /* RS=0 for command register            */
 IO1CLR = RW;                           /* R/W=0 for write                      */
 Temp   = (command1 & 0x0F) << 20;      /* Taking the second nibble of command  */
 IO1SET = IO1SET | Temp;                /* Writing it to data line              */
 Delay(2);
 IO1CLR = EN;                           /* Enable pin low to give H-L pulse     */
}

/********************************************************************************
* Function    : LCD_Data                                                        *
*                                                                               *
* Description : Function to display single character on LCD                     *
*                                                                               *
* Parameters  : data - character to be displayed                                *
*********************************************************************************/
 void LCD_Data(char data)  
{
 int Temp;
 IO1CLR = CLR;                          /* Clearing the port pins               */
 IO1SET = EN;                           /* Enable pin high                      */
 IO1SET = RS;                           /* RS=1 for data register               */
 IO1CLR = RW;                           /* R/W=0 for write                      */
 Temp   = (data & 0xF0) << 16;          /* Taking the first nibble of command   */
 IO1SET = IO1SET | Temp;                /* Writing it to data line              */
 Delay(2);
 IO1CLR = EN;                           /* Enable pin low to give H-L pulse     */

 IO1CLR = CLR;                          /* Clearing the port pins               */
 IO1SET = EN;                           /* Enable pin high                      */
 IO1SET = RS;                           /* RS=1 for data register               */
 IO1CLR = RW;                           /* R/W=0 for write                      */
 Temp   = (data & 0x0F)<<20;            /* Taking the second nibble of command  */
 IO1SET = IO1SET | Temp;                /* Writing it to data line              */
 Delay(2);
 IO1CLR = EN;                           /* Enable pin low to give H-L pulse     */
}

/********************************************************************************
* Function    : LCD_String                                                      *
*                                                                               *
* Description : Function to display string on LCD                               *
*                                                                               *
* Parameters  : String to be displayed                                          *
*********************************************************************************/ 
void LCD_String(unsigned char *dat)
{
 while(*dat!='\0')                      /* Check for termination character      */
 {
  LCD_Data(*dat);                       /* Display the character on LCD         */
  dat++;                                /* Increment the pointer                */
 }
}

/********************************************************************************
* Function    : LCD_Init                                                        *
*                                                                               *
* Description : Function to initialize LCD                                      *
*                                                                               *
* Parameters  : None                                                            *
*********************************************************************************/
void LCD_Init(void)
{
 Delay(15);
 LCD_Command(0x30);
 Delay(10);	
 LCD_Command(0x30);
 Delay(5);	
 LCD_Command(0x30);
 LCD_Command(0x20);
 LCD_Command1(0x28);
 LCD_Command1(0x01);                    /* Clear display                        */
 LCD_Command1(0x06);                    /* Auto increment                       */
 LCD_Command1(0x0C);                    /* Cursor off                           */
}

/********************************************************************************
* Function    : ADC_Conversion                                                  *
*                                                                               *
* Description : Function to get ADC reading                                     *
*                                                                               *
* Parameters  : None                                                            *
*********************************************************************************/
int ADC_Conversion()
{
 int ab;                                /* Variable to store ADC value          */
 Delay(1);
 AD0CR = AD0CR|0x01000000;              /* Start conversion                     */
 while((AD0DR&0x80000000)!=0x80000000); /* Wait here till conversion is over    */ 
 ab    = (AD0DR&0x0000FFC0);            /* Extracting the result                */
 ab    = (ab>>6);                       /* Shift 6 bits right                   */
 return ab;                             /* Return the result                    */
}

/********************************************************************************
* Function    : Int_ASCII                                                       *
*                                                                               *
* Description : Function to store integer value in array                        *
*                                                                               *
* Parameters  : value - value to be stored                                      *									
*               cnt   - no: of digits                                           *
*********************************************************************************/
void Int_ASCII(int value,char cnt)
{
 int i = 0;                             /* Local variables                      */
 char array[7];
 int values;
 values= value;

 for(i=1;i<=cnt;i++)                    /* Store the received value in array    */
 {
  array[i] = values%10;
  values   = values/10;
 }

 for(i=cnt;i>=1;i--)                    /* Display it on LCD                    */
 {
  LCD_Data(array[i]+'0');
 }
}

/********************************************************************************
* Function    : ADC_Check                                                       *
*                                                                               *
* Description : Function to take ADC reading & display it on LCD                *
*                                                                               *
* Parameters  : None                                                            *
*********************************************************************************/
void ADC_Check()
{
 AD0CR=0x00200604;                      /* PDN=1,CLKDIV=6,channel=AD0.2         */
 LCD_Command1(0x80);
 LCD_String("ADC O/P In Terms");
 LCD_Command1(0xC0);
 LCD_String("of Vol=");
 adc_value=ADC_Conversion();            /* Get the result of conversion         */
 adc_value=3.2*adc_value;
 LCD_Command1(0xC7);                    /* 2nd row, 5th location                */
 Int_ASCII(adc_value,4);                /* Display the result on LCD            */
 LCD_String("mV");
 Delay(100);
}
/********************************************************************************
* Function    : PORT_Initial                                                    *
*                                                                               *
* Description : Function for PORT Initialization                                *
*                                                                               *
*********************************************************************************/
void PORT_Initial(void)    
{
 IO1DIR   =    0x00FE0000;              /* LCD pins set as o/p                  */
 PINSEL0  =    0x00000000;
 PINSEL1  =    0x05000000;              /* ADC function for 0.28 & 0.29         */
 PINSEL2  =    0x00000000;
}
/********************************************************************************
                              MAIN FUNCTION
*********************************************************************************/	
int main()
{
 PORT_Initial();                        /* Initialize the ports                 */
 LCD_Init();                            /* Initialize LCD                       */
 LCD_String("Analog 2 Digital");
 LCD_Command1(0xC0);                    /* 2nd row, 1st location                */
 LCD_String("   Conversion   ");
 Delay(500);
 LCD_Command1(0x01);                    /* Clear screen                         */
 while(1)
 {
  ADC_Check();                          /* Take ADC reading                     */
 }
}
