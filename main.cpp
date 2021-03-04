#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

#define COMPORT "\\\\.\\COM4"

HANDLE hComm;

#define WIDTH 1600
#define HEIGHT 1200


void start_sig()
{
   printf("send start signal\n");
   char c = 'a';
   DWORD nobytesread=0;
   WriteFile(hComm, &c, 1, &nobytesread,NULL);
   if(nobytesread!=1)
   {
      printf("failed\n");
      exit(1);
   }
}

int main()
{

  hComm = CreateFile(COMPORT, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

   if (hComm == INVALID_HANDLE_VALUE)
   {
      printf("%s NOT VALID\n", COMPORT);
   }

   DCB dcb = {0};
   dcb.DCBlength = sizeof(DCB);
   if (!GetCommState (hComm,&dcb))
   {
      printf("GetCommState failed\n");
      return 1;
   }
   dcb.BaudRate = 1000000;
   dcb.ByteSize = 8;
   dcb.StopBits = ONESTOPBIT;
   dcb.Parity = NOPARITY;
   SetCommState(hComm, &dcb);
   SetCommMask(hComm, EV_RXCHAR|EV_RXFLAG);

   Uint8 pdata[1600];
   DWORD nobytesread=0;

   Uint8 pixel[1920000];

   SYSTEMTIME st;
   GetSystemTime(&st);
   int timepast=st.wSecond;
   int total=0, totalpast=0;
   printf("enter mainloop\n");

   FILE *jpegfile ;

   int valid_data=0;
   int filecnt=0;

   start_sig();
   while(1)
   {
//      DWORD CommEvt;
//      WaitCommEvent(hComm,&CommEvt,NULL);

      ReadFile(hComm, pdata, 1024, &nobytesread, NULL);
      if(nobytesread>0)
      {

//         if(valid_data==1)
//         {
//            total += nobytesread;
//            GetSystemTime(&st);
//            if(st.wSecond != timepast)
//            {
//               printf("\r%10d | %5d bytes\t\t", total, total-totalpast);
//               totalpast = total;
//               timepast = st.wSecond;
//            }
//            fwrite(pdata, 1, nobytesread, jpegfile);
//         }
//         for(int i=0;i<nobytesread-1;i++)
//         {
//            if(pdata[i]==0xFF&&pdata[i+1]==0xD8)
//            {
//               valid_data=1;
//
//               printf("\nstart!!\n");
//               char c[100];
//               sprintf(c,"img%d.jpg", ++filecnt);
//               jpegfile = fopen(c, "wb");
//               fwrite(&pdata[i], 1, nobytesread-i, jpegfile);
//               break;
//            }
//            else if(pdata[i]==0xFF&&pdata[i+1]==0xD9)
//            {
//               if(valid_data)
//               {
//                  fwrite(pdata, 1, i+2, jpegfile);
//                  fflush(jpegfile);
//                  fclose(jpegfile);
//                  printf("\nJPEG saved \n");
//                  total=totalpast=0;
//               }
//               valid_data=0;
//               break;
//            }
//         }



         memcpy(&pixel[total],pdata,nobytesread);
         total += nobytesread;
         GetSystemTime(&st);
         if(st.wSecond != timepast)
         {
            printf("\r%10d | %5d bytes\t\t", total, total-totalpast);
            totalpast = total;
            timepast = st.wSecond;
         }
         if((WIDTH*HEIGHT)==total)
         {
            printf("\r%10d | %5d bytes\t\t", total, total-totalpast);
            total=totalpast=0;

//            FILE *file = fopen("pic.bin", "wb");
//            fwrite(pixel, 1, sizeof(pixel), file);
//            fflush(file);
//            fclose(file);

            Image img;
            img.create(WIDTH,HEIGHT, Color::Black);
            for(int h=0;h<HEIGHT;h++)
            {
               for(int w=0;w<WIDTH;w++)
               {
                  Uint8 color;
                  color = pixel[(h*WIDTH)+w];
                  img.setPixel(w,h,Color(color,color,color));
               }
            }
            img.saveToFile("pic.jpg");
            printf("\n");
            start_sig();
         }


      }
   }

   CloseHandle(hComm);//Closing the Serial Port
   return 0;
}
