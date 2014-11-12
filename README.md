arenib_interface
================

Interface de debug/visu du robot

Envoyer des paquets à ce serveur en UDP sur le port 2222 
suivant ce protocole:

	sf::Packet packet;
	packet << (sf::Uint8) 0x22;  //magic       // uint8
	packet << std::string("Nom de mon robot"); // std::string
	packet << (sf::Uint8) etat;                // uint8
	packet << (sf::Int16) position_x; //mm     // int16
	packet << (sf::Int16) position_y; //mm     // int16
	packet << (sf::Int16) theta; //degres*10   // int16
	packet << (sf::Uint8) color_r; //rouge     // uint8
	packet << (sf::Uint8) color_g; //vert      // uint8
	packet << (sf::Uint8) color_b; //bleu      // uint8
	
	packet << extra;  //vous pouvez ajouter des données relative à votre robot
	
	udpSocket.send(packet, addresseIpDeLinterface, 2222);
	
Voilà c'est aussi simple que cela...
Si vous voulez visualiser des infos complémentaires il faut 
implémenter votre robot dans l'interface...

	