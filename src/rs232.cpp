
#include "rs232.hpp"
#include <stdio.h>

RS232::RS232(std::string port,BYTE parity,int Vitesse,int Data, BYTE stop,int TimeOut)
{
  _mutex.lock();
  _port_str=port;
  DWORD dwError;					// n° de l'erreur
  BOOL flag_etat;					// tout c'est bien passé

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
    /* Fichier non créer gérer l'erreur */
    MessageBoxA(0,"Erreur Serial Port", "Error Creating File Serial Port",MB_OK);
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
  _timeout.ReadTotalTimeoutConstant = TimeOut; // pas de time out = 0
  _timeout.WriteTotalTimeoutMultiplier = 0;
  _timeout.WriteTotalTimeoutConstant = 0;

// configurer le timeout

  SetCommTimeouts(_handle,&_timeout);

/*-----------------------------------------------*/
/* Configurer le port	                          */
/*-----------------------------------------------*/
  flag_etat = SetCommState(_handle, &_config);

  if (!flag_etat) {
    MessageBoxA(0,"Erreur Serial Port", "Error SetCommState Serial Port",MB_OK);
  }
  _mutex.unlock();
}

int RS232::send(const char* buffer, unsigned int size)
{
  BOOL flag_etat;
  unsigned long nb_octets_wr;
  _mutex.lock();
  flag_etat = WriteFile(_handle,buffer,size,&nb_octets_wr,NULL);
  _mutex.unlock();
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
  _mutex.lock();
  flag_etat = ReadFile(_handle,buffer,size,&nBytesRead,NULL);
  _mutex.unlock();
  if (flag_etat)
  {
    return nBytesRead;
  }
  MessageBoxA(0,"Error Serial Port", "Recv Error", MB_OK);
  return -1;
}



