#ifndef _RS232
#define _RS232

#include <windows.h>
#include <SFML/System.hpp>

class RS232 
{
private:
    //Variables de la communication
    DCB _config;
    HANDLE _handle;
    COMMTIMEOUTS _timeout;

    //var des threads
    sf::Mutex _mutex;
    int _maxsize;      //taille maximale de la donnee un octets

public:

/*Description-------------------------------------------------
*                 Fonction d'initialisation d'un port RS232
*
* Parametres
*  char *Port    	: "COM1" pour Port1, "COM2" pour port2
*  char *Parite  	: "Paire", "Impaire", "Aucune"
*  int Vitesse          : la vitesse en bauds
*  int Data             : le nombre de bit de donnée
*  char *StopBit 	: Le nombre de stop Bit "1" ,"1.5","2"
*  int TimeOut		: Temps alloué pour la lecture de la trame de retour (en ms) par d�faut : 500ms
*
* Mise en place du thread, le flag passe devient non nul quand qq chose arrive
* Se qui arrive est alors mis dans reception
*
* Retour : un entier egal �  0 si erreur.
------------------------------------------------------------------------	*/

    RS232(char *Port="COM1",char *Parite="Aucune",int Vitesse=9600,int Data=8,char *StopBit="1",int TimeOut=500);
    ~RS232(){CloseHandle(_handle);}

/*Description-------------------------------------------------
*                 Fonction d'�criture sur un port RS232
*
* Param�tres
*  char *Port		:"COM1" pour Port1, "COM2" pour port2
* char *Chaine          :Chaine �  envoyer sur le port
*  char *EolChar	:Caract�re de fin de chaine"CR","LF","LF/CR","CR/LF", si autre chose, ou rien, se sera :'$'
*
* Retour : un entier egal �  0 si erreur.
*------------------------------------------------------------------------	*/
    int envoi_chaine(char *Chaine,char *EolChar = "$");


/*-Description------------------------------------------------
 * 					Fonction de lecture sur le port série
 * Param�tres
 * char* Port 			:"COM1" ou "COM2"
 * char* ChaineRecue            :Pointeur vers la chaine re�ue
 *
 * NB :!! Ici un timer est utilise pour avoir une fenetre de reception
            Il est r�gl� dans le constructeur

 Le retour est un entier nul si erreur
 * ----------------------------------------------------------------------*/
    int recevoir(char *ChaineRecue);


};
#endif

