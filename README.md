ARENIB INTERFACE
================
© Copydown - Julien De Loor pour la coupe eurobot

Interface de debug/visu haut niveau d'un ou plusieurs robots.
Extrêmement facile à utiliser, on peut utiliser un simulateur 
ou un robot réel (à condition d'avoir du wifi dans le robot).

CECI N'EST PAS UN SIMULATEUR !!!

Pour l'utiliser c'est encore une fois extrêmement facile:
Envoyez des paquets à ce serveur en UDP sur le port 2222 
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
implémenter votre robot dans l'interface (voir plus bas)...

Requiert la SFML 2.1 ou 2.0 pour compiler (et c'est TOUT !).


Afficher son Robot
------------------

Copiez-coller la classe Robot et changer les Sf::Shapes.
La convention ARENIB est la suivante:

	Axe X => Axe des roues
	Axe Y => Axe perpendiculaire à l'axe des roues (Direction "avant" du Robot)
	Orientation des axes suis l'orientation mathématique (sens Trigonométrique)
	c'est donc en mirroir avec l'axe X par rapport à SFML.

	
	
	







	