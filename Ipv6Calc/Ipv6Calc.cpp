 // Ipv6Calc.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <string> 
#include <regex>
#include "IPv6.h"

void help()
{
	std::cout << "Kalkulator adresow IPv6" << '\n' << '\n'
		<< "Parametry:" << '\n'
		<< "-a - adres IPv6" << '\n'
		<< "-m - maska" << '\n'
		<< "SKLADNIA: IPv6Calc -a [ADRES] -m [MASKA]";
}

int main(int argc, char * argv[])
{
	/*jesli jest niewystarczajaca ilosc argumentow, wyswietl pomoc i zakoncz program*/
	if (argc <= 4)
	{
		help();
		return 1;
	}

	char inputIP[40];   //wejsciowy adres IP
	int mask = atoi(argv[4]); //maska sieci
	std::regex wzorzec("^((([0-9A-Fa-f]{1,4}:){7}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){6}:[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){5}:([0-9A-Fa-f]{1,4}:)?[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){4}:([0-9A-Fa-f]{1,4}:){0,2}[0-9A-Fa-f]{1,})|(([0-9A-Fa-f]{1,4}:){3}:([0-9A-Fa-f]{1,4}:){0,3}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){2}:([0-9A-Fa-f]{1,4}:){0,4}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){6}((\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\b)\\.){3}(\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\b))|(([0-9A-Fa-f]{1,4}:){0,5}:((\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\b)\\.){3}(\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\b))|(::([0-9A-Fa-f]{1,4}:){0,5}((\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\b)\\.){3}(\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\b))|([0-9A-Fa-f]{1,4}::([0-9A-Fa-f]{1,4}:){0,5}[0-9A-Fa-f]{1,4})|(::([0-9A-Fa-f]{1,4}:){0,6}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){1,7}:))$");


	/*sprawdzanie argumentow i przypisanie do zmiennych - zakoncz program jesli sie nie zgadza wg wzorca*/

	/*walidacja adresu IP*/
	if (strcmp("-a", argv[1]) == 0)
	{
		if (regex_match(argv[2], wzorzec))
			strcpy_s(inputIP, argv[2]);
		else
		{
			std::cout << "Nieprawidlowy adres IP" << "\n";
			return 2;
		}

	}
	else
		return 2;

	/*walidacja maski*/
	if (strcmp("-m", argv[3]) == 0)
	{
		if (atoi(argv[4]) > 0 && atoi(argv[4]) <= 128)
			mask = atoi(argv[4]);
		else
		{
			std::cout << "Nieprawidlowa maska" << "\n";
			return 2;
		}
	}
	else
		return 2;

    IPv6 ip = IPv6();  //inicjalizacja biblioteki


    ip.setAddress(inputIP);                //ustawienie adresu IP

    ip.setNetMask(mask);                      //ustawienie maski sieci

    ip.makeNetworkAddress();                //konwersja adresu IP na adres sieci


    char minaddress[40] = { 0 };            //zmienne testowe
    uint16_t address[8];


    ip.getNetwork(address);
    ip.getShortAddress(address, minaddress);
    std::cout << "Adres sieci: " << minaddress << '\n';

	memset(address, 0, sizeof address);             //czyszczenie zmiennych tablic
	memset(minaddress, 0, sizeof minaddress);
	ip.getNetwork(address);
	ip.getLongAddress(address, minaddress);
	std::cout << "Adres sieci z rozwinieciem zer: " << minaddress << '\n';
	std::cout << "Maska sieci: " << std::to_string(ip.getNetmask()) << '\n';

    memset(address, 0, sizeof address);
    memset(minaddress, 0, sizeof minaddress);
    ip.getMinAddress(address);
    ip.getShortAddress(address, minaddress);
    std::cout << "Najniższy adres w danej sieci: " << minaddress<< '\n';


    memset(address, 0, sizeof address);
    memset(minaddress, 0, sizeof minaddress);
    ip.getMaxAddress(address);
    ip.getShortAddress(address, minaddress);
    std::cout << "Najwyższy adres w danej sieci: " << minaddress << '\n';


    std::cout << "Ilość hostów w sieci: " << ip.getMaxClients() << '\n';

    char netAddress[50] = { 0 };            //zmienne testowe
    memset(address, 0, sizeof address);
    ip.getNetwork(address);
    ip.getShortAddress(address, netAddress);

    memset(address, 0, sizeof address);
    ip.getPrevNetwork(address);
    char prevAddress[50] = { 0 };
    ip.getShortAddress(address, prevAddress);


    memset(address, 0, sizeof address);
    ip.getNextNetwork(address);
    char nextAddress[50] = { 0 };
    ip.getShortAddress(address, nextAddress);

    ip.diff3Address(prevAddress, netAddress, nextAddress);

    std::cout << "Porównanie poprzedniej z aktualną i następną siecią: \n" << prevAddress << '/' << std::to_string(ip.getNetmask()) << '\n';
    std::cout << netAddress << '/' << std::to_string(ip.getNetmask()) << '\n';
    std::cout << nextAddress << '/' << std::to_string(ip.getNetmask()) << '\n';


    return 0;
}
