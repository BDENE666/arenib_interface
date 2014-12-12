
#include "rs232.hpp"
#include <stdio.h>

RS232::RS232(char *Port,char *Parite,int Vitesse,int Data,char *StopBit,int TimeOut)
{
    _mutex.lock();

    _maxsize = 20;

    DWORD dwError;					// n° de l'erreur
    BOOL flag_etat;					// tout c'est bien passé
    //wchar_t *pwc      = (wchar_t *)malloc( sizeof( wchar_t )*5);

/*--------------------------------------------------------*/
/*                 Ouverture du port de Com               */
/*--------------------------------------------------------*/
 
//mbstowcs(pwc, Port,5 );

        _handle = CreateFileA(
                Port,                         	// Choix du port « COM »
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
                }

/*-----------------------------------------------------------*/
/* Lecture Configuration initiale                            */
/*-----------------------------------------------------------*/
      int creation_ok = GetCommState(_handle, &_config);

/*-------------------------------------------------------------------*/
/* 					Configuration du port                            */
/*-------------------------------------------------------------------*/

// Gestion de la vitesse
      _config.BaudRate = Vitesse;

// Gestion du nombre de bits
      _config.ByteSize = Data;

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

        _mutex.unlock();
}

int RS232::envoi_chaine(char *Chaine,char *EolChar)
{
        BOOL flag_etat;
        int nb_octets;
        char *Message;
        unsigned long nb_octets_wr;

        Message = new char[_maxsize];

 /*-------------------------------------------------
          Envoi de la chaine + caractere de stop
 --------------------------------------------------*/
        if(strcmp(EolChar,"CR")==0)
                sprintf(Message,"%s%c",Chaine,0x0D);

        else if(strcmp(EolChar,"LF")==0)
                sprintf(Message,"%s%c",Chaine,0x0A);

        else if(strcmp(EolChar,"LF/CR")==0)
                sprintf(Message,"%s%c%c",Chaine,0x0A,0x0D);

        else if(strcmp(EolChar,"CR/LF")==0)
                sprintf(Message,"%s%c%c",Chaine,0x0D,0x0A);

        else
                sprintf(Message,"%s%c",Chaine,'$');

// compter le nombre d'octets a envoyer
        nb_octets = strlen(Message);

// ecrire dans le fichier
        _mutex.lock();
        flag_etat = WriteFile(_handle,Message,nb_octets,&nb_octets_wr,NULL);
        _mutex.unlock();

// Liberation memoire
        delete[] Message;

        return(flag_etat);
}



int RS232::recevoir(char *ChaineRecue)
{
        BOOL flag_etat;
        unsigned long nBytesRead;
        char *buffer;

 /*-------------------------------------*/
 /* Initialisation des variables        */
 /*-------------------------------------*/

        buffer  = new char[_maxsize-1];
        buffer[0] = '\0';
        nBytesRead=0;

 /*-------------------------------------------------------------*/
 /* boucle d'attente de lecture des octets                      */
 /* Sortie de la boucle par timeout par exemple, si             */
 /*-------------------------------------------------------------*/
        _mutex.lock();
        flag_etat = ReadFile(_handle,buffer,_maxsize,&nBytesRead,NULL);
        _mutex.unlock();

        buffer[nBytesRead] = '\0';          //Car pas forcément renvoyé par le système relié sur le port série

        sprintf(ChaineRecue,"%s",buffer);   // Retourner la chaine recue

// Liberer la memoire
        delete[] buffer;

        return(flag_etat);
}



