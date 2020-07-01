#pragma once
#include <stdint.h>
#include <boost/multiprecision/cpp_int.hpp>

/**
 * .
 */
class IPv6
{
private:
	/**
	 * Tablica adresu ip zapisanego jako string zawieraj¹ca osiem segmentów adresu w pe³nym zapisie.
	 */
	char charIpSegment[8][5];
	/**
	 * Zienna zawieraj¹ca zapis dziesiêtny masko podsieci.
	 */
	uint8_t netMask;
	/**
	 * Tablica maski sieci zapisana w oœmiu segmentach binarnych.
	 */
	uint16_t binaryNetMask[8] = { 0 };
	/**
	 * Tablica adresu IP zapisana w oœmiu segmentach binarnych.
	 */
	uint16_t binaryIp[8] = { 0 };
	/**
	 * Tablica adresu IP sieci zapisana w oœmiu segmentach binarnych.
	 */
	uint16_t binaryNetworkIp[8] = { 0 };
	/**
	 * Funkcja odpowiedzialna za przemiane adresu IP na pe³ny adres IP jako string oraz zamianê na adres binarny.
	 * 
	 * \param Ci¹g znaków zawieraj¹cych adres IP
	 */
	void validateAddress(char*);
	/**
	 *  PL: Funkcja zamieniaj¹ca maskê z wartoœci dziesiêtnej na maskê w zapisie binarnym.
	 * 
	 */
	void convertDecToBinMask();
	/**
	 * Funkcja zamieniaj¹ca adres IP z ci¹gu znaków na adres binarny.
	 * 
	 */
	void convertCharToBinAddress();
	/**
	 * Funckja konwertuj¹ca wartoœæ binarn¹ na znak.
	 * 
	 * \param Bajt =, który ma byæ zamieniony
	 * \return znak zamienionego bajtu
	 */
	char hexToAscii(uint8_t);
	/**
	 * Funkcja zamienia znak ascii na wartoœæ binarn¹.
	 * 
	 * \param znak do zamiany
	 * \return wartoœæ binarna
	 */
	uint8_t asciiToHex(char);
	



public:

	/**
	 * Wpisanie adresu do kalsy.
	 * 
	 * \param Adres IP zapisany jako ci¹g znaków
	 */
	void setAddress(char *);

	/**
	 * Ustawienie maski sieci oraz konwersja na adres binarny.
	 * 
	 * \param Wartoœæ dziesiêtna maski
	 */
	void setNetMask(uint8_t);

	/**
	 * Przetwarza podany adres na adres sieci na podstawie podanej maski.
	 * 
	 */
	void makeNetworkAddress();

	/**
	 * Zwraca adres IP o najni¿szym numerze w podanej sieci.
	 * 
	 * \param [OUT] tablica zawieraj¹ca adres sieci, tablica formatu uint16_t[8].
	 */
	void getMinAddress(uint16_t*);

	/**
	 * Zwraca adres IP o najwy¿szym numerze w podanej sieci.
	 * 
	 * \param [OUT] tablica zawieraj¹ca adres sieci, tablica formatu uint16_t[8].
	 * 
	 */
	void getMaxAddress(uint16_t*);

	/**
	 * Zwraca adres s¹siaduj¹cej sieci o numerze ni¿szym.
	 * 
	 * \param [OUT] tablica zawieraj¹ca adres sieci, tablica formatu uint16_t[8].
	 */
	void getPrevNetwork(uint16_t*);

	/**
	 * Zwraca adres s¹siaduj¹cej sieci o numerze wy¿szym.
	 * 
	 * \param [OUT] tablica zawieraj¹ca adres sieci, tablica formatu uint16_t[8].
	 */
	void getNextNetwork(uint16_t*);

	/**
	 * Zwraca adres sieci w postaci binarnej.
	 * 
	 * \param [OUT] tablica zawieraj¹ca adres sieci, tablica formatu uint16_t[8].
	 */
	void getNetwork(uint16_t*);

	/**
	 * Zwraca ci¹g znaków zawieraj¹cych adres IP sieci, adres w postaci pe³nej  8 segmentów po 4 znaki
	 * 
	 * \param Tablica zawieraj¹ca adres w postaci binarnej uint16_t[8].
	 * \param [OUT] Tablica znaków w której zostanie zapisany adres IP, minimum 40 elemetów tablicy.
	 */
	void getLongAddress(uint16_t[], char*);

	/**
	 * Zwraca ci¹g znaków zawieraj¹cych adres IP sieci, adres w postaci skróconej.
	 * 
	 * \param Tablica zawieraj¹ca adres w postaci binarnej uint16_t[8].
	 * \param [OUT] Tablica znaków w której zostanie zapisany adres IP, minimum 40 elemetów tablicy.
	 */
	void getShortAddress(uint16_t[], char*);

	/**
	 * Funkcja zwraca maksymaln¹ iloœæ hostów, które mog¹ nale¿eæ do sieci przy zak³ddanej masce.
	 * 
	 * \return Maksymalna iloœæ hostów w sieci
	 */
	boost::multiprecision::uint128_t getMaxClients();

	/**
	 * 
	 * 
	 * \return d³ugoœæ maski sieci w zapisie dziesiêtnym
	 */
	uint8_t getNetmask();
	/**
	 * Zwraca ci¹gi znaków zawieraj¹cych adres IP sieci z wyznaczonymi ró¿nicami.
	 *
	 * \param [IN/OUT] Tablica znaków w której zostanie zapisany adres IP, minimum 50 elemetów tablicy.
	 * \param [IN/OUT] Tablica znaków w której zostanie zapisany adres IP, minimum 50 elemetów tablicy.
	 * \param [IN/OUT] Tablica znaków w której zostanie zapisany adres IP, minimum 50 elemetów tablicy.
	 */
	void diff3Address(char*, char*, char*);


};

