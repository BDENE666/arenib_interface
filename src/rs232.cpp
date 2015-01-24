
#include "rs232.hpp"
#include <stdio.h>
#include <sstream>
#include <iostream>

#include <robot.hpp>

RS232::RS232(std::string port,BYTE parity,int Vitesse,int Data, BYTE stop,int TimeOut) :
_port_str(port),
_robot(0),
_thread(&RS232::thread_func,this),
_ragequit(false)
{
  inStruct=0;
  _port_str=port;
  DWORD dwError;					// n° de l'erreur
  BOOL flag_etat;					// tout c'est bien passé
  
  /*if (port.size() && port[0] != '\\') {
    port=std::string("\\\\") + port;
  }*/

/*--------------------------------------------------------*/
/*                 Ouverture du port de Com               */
/*--------------------------------------------------------*/
  _handle = CreateFileA(port.c_str(),                         	// Choix du port « COM »
                        GENERIC_READ | GENERIC_WRITE, 	// accès pour lire et écrire sur le port
                        0,                            	// accès exclusif au port de COM
                        NULL,                         	// sécurité par défaut
                        OPEN_EXISTING,                	// Doit être à cette valeur car ce n'est pas un fichier
                        0,              		// mode asynchrone
                        NULL);

/*-----------------------------------------------------------*/
/*         Vérifier si handle ouvert correctement            */
/*-----------------------------------------------------------*/
  if (_handle == INVALID_HANDLE_VALUE)
  {
    dwError = GetLastError();
    std::stringstream s;
    s << "Erreur Serial Port : " << dwError << std::endl;
    /* Fichier non créer gérer l'erreur */
    MessageBoxA(0,s.str().c_str(), "Error Creating File Serial Port",MB_OK);
  }

/*-----------------------------------------------------------*/
/* Lecture Configuration initiale                            */
/*-----------------------------------------------------------*/
  GetCommState(_handle, &_config);

/*-------------------------------------------------------------------*/
/* 					Configuration du port                            */
/*-------------------------------------------------------------------*/

// Gestion de la vitesse
  _config.BaudRate = Vitesse;

// Gestion du nombre de bits
  _config.ByteSize = Data;

  _config.Parity = parity;
  _config.StopBits = stop;
      
      /*
// Gestion de la parité
        if (strcmp(Parite,"Aucune")==0)
                _config.Parity = NOPARITY;

        if (strcmp(Parite,"Paire")==0)
                _config.Parity = EVENPARITY;

        if (strcmp(Parite,"Impaire")==0)
                _config.Parity = ODDPARITY;

// Gestion du Stop Bit
        if (strcmp(StopBit,"2")==0)
                _config.StopBits = TWOSTOPBITS;

        else if (strcmp(StopBit,"1.5")==0)
                _config.StopBits = ONE5STOPBITS;

        else
                _config.StopBits = ONESTOPBIT;
*/
// configuration.configurationlength;

  _config.fBinary = 1;
  _config.fParity = 0;
  _config.fOutxCtsFlow = 0;
  _config.fOutxDsrFlow = 0;
  _config.fDtrControl = 0;
  _config.fDsrSensitivity = 0;
  _config.fTXContinueOnXoff = 0;
  _config.fRtsControl = 0;

/*----------------------------------*/
/*    Définition des timeouts       */
/*----------------------------------*/

  

  _timeout.ReadIntervalTimeout = MAXWORD;
  _timeout.ReadTotalTimeoutMultiplier = 0;
  _timeout.ReadTotalTimeoutConstant = 1; // pas de time out = 0
  _timeout.WriteTotalTimeoutMultiplier = 0;
  _timeout.WriteTotalTimeoutConstant = 0;
/*
  _timeout.ReadIntervalTimeout=50;
  _timeout.ReadTotalTimeoutConstant=50;
  _timeout.ReadTotalTimeoutMultiplier=10;
  _timeout.WriteTotalTimeoutConstant=50;
  _timeout.WriteTotalTimeoutMultiplier=10;
  */
// configurer le timeout

  SetCommTimeouts(_handle,&_timeout);

/*-----------------------------------------------*/
/* Configurer le port	                          */
/*-----------------------------------------------*/
  flag_etat = SetCommState(_handle, &_config);

  if (!flag_etat) {
    MessageBoxA(0,"Erreur Serial Port", "Error SetCommState Serial Port",MB_OK);
  }
  //_thread.launch();
}

int RS232::send(const char* buffer, unsigned int size)
{
  BOOL flag_etat;
  unsigned long nb_octets_wr;
  flag_etat = WriteFile(_handle,buffer,size,&nb_octets_wr,NULL);
  if (flag_etat)
  {
    return nb_octets_wr;
  }
  MessageBoxA(0,"Error Serial Port", "Send Error", MB_OK);
  return -1;
}



int RS232::recv(char* buffer, unsigned int size)
{
  BOOL flag_etat;
  unsigned long nBytesRead;

  flag_etat = ReadFile(_handle,buffer,size,&nBytesRead,NULL);

  if (flag_etat)
  {
    return nBytesRead;
  }
  MessageBoxA(0,"Error Serial Port", "Recv Error", MB_OK);
  return -1;
}

RS232::~RS232()
{
  _ragequit=true; 
  _thread.wait(); 
  CloseHandle(_handle);
}

void RS232::update()
{
  unsigned char buf;
  unsigned long nBytesRead=0;
  
  sf::Uint16 temp;
  while (ReadFile(_handle,&buf,1,&nBytesRead,NULL) && nBytesRead)
  {
  //std::cout << buf << std::endl;
  //printf("recv 0x%x\n",buf);
    if (inStruct)
    {
      
    /*     flags;              // uint16 
    packet << (sf::Uint8) etat;                // uint8 
    packet << (sf::Int16) position_x; //mm     // int16 
    packet << (sf::Int16) position_y; //mm     // int16 
    packet << (sf::Int16) theta; //degres*10   // int16 
    packet << (sf::Uint8) color_r; //rouge     // uint8 
    packet << (sf::Uint8) color_g; //vert      // uint8 
    packet << (sf::Uint8) color_b; //bleu      // uint8 
    */
    
    //if (buf == 0xFF) buf=0; //trashfix bug puts
    
      //std::cout << "inStruct " << inStruct << std::endl;
      switch (inStruct)
      {
        //nb_robots
        case 1: break;
        //flag
        case 2: temp = buf << 8; break;
        case 3: temp |= buf; packet << temp; break;
        //etat
        case 4: packet << buf; break;
        //position_x
        case 5: temp = buf << 8;  break;
        case 6: temp |= buf; packet << temp; /*std::cout << "Position X " << temp << std::endl; */break;
        //position_y
        case 7: temp = buf << 8; break;
        case 8: temp |= buf; packet << temp;  /*std::cout << "Position Y " << temp << std::endl; */break;
        //theta
        case 9: temp = buf << 8; break;
        case 10: temp |= buf; packet << temp; /*std::cout << "Theta X " << temp << std::endl;*/ break;
        //color
        case 11: packet << buf; break;
        case 12: packet << buf; break;
        case 13: packet << buf; 
          packet << buf; 
          packet << buf; 
          _mutex.lock();
          if (_robot) {
            //std::cout << "EXTRACT" << std::endl;
            _robot->extract(packet);
            packet.clear();
          }
          _mutex.unlock();
        default:
          inStruct=0;
          break;
      }
      if (inStruct) inStruct++;
    }
    else {
      if (buf == 022) {
        inStruct++;
      }
      else if (buf=='\n' || buf=='\r' )
      {
        std::cout << "[" << _port_str << "] " << console << std::endl;
        console="";
      }
      else console += buf;
    }
  }
}

void RS232::thread_func()
{
  
}