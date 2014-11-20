ARENIB INTERFACE
================
© Copydown - Julien De Loor pour la coupe eurobot

Requiert la SFML 2.1 ou 2.0 pour compiler (et c'est TOUT !).

Interface de debug/visu haut niveau d'un ou plusieurs robots.
Extrêmement facile à utiliser, on peut utiliser un simulateur 
ou un robot réel (à condition d'avoir du wifi dans le robot).

CECI N'EST PAS UN SIMULATEUR !!!

Pour l'utiliser c'est encore une fois extrêmement facile:
Envoyez des paquets à ce serveur en UDP sur le port 2222 
suivant ce protocole:

	sf::Packet packet;
	packet << (sf::Uint8) 0x22;  //magic       // uint8
	packet << (sf::Uint8) 0x1;                 // uint8 <-- "nombre de robot"
	packet << (sf::Uint16) flags;              // uint16 
	packet << std::string("Nom de mon robot"); // std::string <-- identifiant du robot gardez tjs le même
	packet << (sf::Uint8) etat;                // uint8 
	packet << (sf::Int16) position_x; //mm     // int16 
	packet << (sf::Int16) position_y; //mm     // int16 
	packet << (sf::Int16) theta; //degres*10   // int16 
	packet << (sf::Uint8) color_r; //rouge     // uint8 
	packet << (sf::Uint8) color_g; //vert      // uint8 
	packet << (sf::Uint8) color_b; //bleu      // uint8 
	
	//Vous pouvez aussi ajouter d'autres variables (optionnelles) 
	packet << (sf::Int16) extra_size;  //données relative à votre robot
	packet << extra;  //données relative à votre robot
	
	udpSocket.send(packet, addresseIpDeLinterface, 2222);
	
  
	
Voilà c'est aussi simple que cela...

Si vous voulez visualiser des infos complémentaires il faut 
implémenter votre robot dans l'interface (voir plus bas)...


Afficher son Robot
------------------

Copiez-coller la classe Robot et changer les Sf::Shapes.
La convention ARENIB est la suivante:

	Axe X => Axe des roues
	Axe Y => Axe perpendiculaire à l'axe des roues (Direction "avant" du Robot)
	Orientation des axes suis l'orientation mathématique (sens Trigonométrique)
	c'est donc en mirroir avec l'axe X par rapport à SFML.


Recevoir la position des autres sur son Robot
---------------------------------------------

Il vous faut activer cette option en insérant la variable 
flags dans votre paquet et ensuite attendre la reception de 
paquets udp du même type de que le précédant sur le port 2222

Vous ne recevrez que les paquets des AUTRES robots

	sf::Packet packet;
	udpSocket.receive(packet, from_ip, from_port);
	packet >> (sf::Uint8) 0x22;  //magic       // uint8
	packet >> (sf::Uint8) nb_robots;           // uint8
	
	for (int i=0; i < nb_robots; i++) {
	  packet >> (sf::Uint16) flags;  //Identique à celui envoyé par votre robot
	  packet >> (std::string) name;                     
	  packet >> (sf::Uint8) etat;
	 
	  packet >> (sf::Int16) position_x; //mm 
	  packet >> (sf::Int16) position_y; //mm 
	  packet >> (sf::Int16) theta; //degres*10 
	 
	  packet >> (sf::Uint8) color_r;
	  packet >> (sf::Uint8) color_g; 
	  packet >> (sf::Uint8) color_b; 
	  if (flags & 0x8000) packet >> extra;
	}
	
	
Flags
-----
La variable flags permet de régler les données envoyés au 
robot et le delay entre 2 envois d'information
Le mot de 16bits de décompose ainsi


	Fort           Faible
	euuu uuss ssss ssss
	

	e: le paquet contient des donnes extras
	u: non utilisé pour l'instant (mettre 0)
	s: delay entre 2 envois (de 1 à 1023 ms) si 0 (par defaut) pas d'envois de données






	