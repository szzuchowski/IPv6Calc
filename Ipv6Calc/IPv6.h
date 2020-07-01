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
	 * Tablica adresu ip zapisanego jako string zawieraj�ca osiem segment�w adresu w pe�nym zapisie.
	 */
	char charIpSegment[8][5];
	/**
	 * Zienna zawieraj�ca zapis dziesi�tny masko podsieci.
	 */
	uint8_t netMask;
	/**
	 * Tablica maski sieci zapisana w o�miu segmentach binarnych.
	 */
	uint16_t binaryNetMask[8] = { 0 };
	/**
	 * Tablica adresu IP zapisana w o�miu segmentach binarnych.
	 */
	uint16_t binaryIp[8] = { 0 };
	/**
	 * Tablica adresu IP sieci zapisana w o�miu segmentach binarnych.
	 */
	uint16_t binaryNetworkIp[8] = { 0 };
	/**
	 * Funkcja odpowiedzialna za przemiane adresu IP na pe�ny adres IP jako string oraz zamian� na adres binarny.
	 * 
	 * \param Ci�g znak�w zawieraj�cych adres IP
	 */
	void validateAddress(char*);
	/**
	 *  PL: Funkcja zamieniaj�ca mask� z warto�ci dziesi�tnej na mask� w zapisie binarnym.
	 * 
	 */
	void convertDecToBinMask();
	/**
	 * Funkcja zamieniaj�ca adres IP z ci�gu znak�w na adres binarny.
	 * 
	 */
	void convertCharToBinAddress();
	/**
	 * Funckja konwertuj�ca warto�� binarn� na znak.
	 * 
	 * \param Bajt =, kt�ry ma by� zamieniony
	 * \return znak zamienionego bajtu
	 */
	char hexToAscii(uint8_t);
	/**
	 * Funkcja zamienia znak ascii na warto�� binarn�.
	 * 
	 * \param znak do zamiany
	 * \return warto�� binarna
	 */
	uint8_t asciiToHex(char);
	



public:

	/**
	 * Wpisanie adresu do kalsy.
	 * 
	 * \param Adres IP zapisany jako ci�g znak�w
	 */
	void setAddress(char *);

	/**
	 * Ustawienie maski sieci oraz konwersja na adres binarny.
	 * 
	 * \param Warto�� dziesi�tna maski
	 */
	void setNetMask(uint8_t);

	/**
	 * Przetwarza podany adres na adres sieci na podstawie podanej maski.
	 * 
	 */
	void makeNetworkAddress();

	/**
	 * Zwraca adres IP o najni�szym numerze w podanej sieci.
	 * 
	 * \param [OUT] tablica zawieraj�ca adres sieci, tablica formatu uint16_t[8].
	 */
	void getMinAddress(uint16_t*);

	/**
	 * Zwraca adres IP o najwy�szym numerze w podanej sieci.
	 * 
	 * \param [OUT] tablica zawieraj�ca adres sieci, tablica formatu uint16_t[8].
	 * 
	 */
	void getMaxAddress(uint16_t*);

	/**
	 * Zwraca adres s�siaduj�cej sieci o numerze ni�szym.
	 * 
	 * \param [OUT] tablica zawieraj�ca adres sieci, tablica formatu uint16_t[8].
	 */
	void getPrevNetwork(uint16_t*);

	/**
	 * Zwraca adres s�siaduj�cej sieci o numerze wy�szym.
	 * 
	 * \param [OUT] tablica zawieraj�ca adres sieci, tablica formatu uint16_t[8].
	 */
	void getNextNetwork(uint16_t*);

	/**
	 * Zwraca adres sieci w postaci binarnej.
	 * 
	 * \param [OUT] tablica zawieraj�ca adres sieci, tablica formatu uint16_t[8].
	 */
	void getNetwork(uint16_t*);

	/**
	 * Zwraca ci�g znak�w zawieraj�cych adres IP sieci, adres w postaci pe�nej  8 segment�w po 4 znaki
	 * 
	 * \param Tablica zawieraj�ca adres w postaci binarnej uint16_t[8].
	 * \param [OUT] Tablica znak�w w kt�rej zostanie zapisany adres IP, minimum 40 elemet�w tablicy.
	 */
	void getLongAddress(uint16_t[], char*);

	/**
	 * Zwraca ci�g znak�w zawieraj�cych adres IP sieci, adres w postaci skr�conej.
	 * 
	 * \param Tablica zawieraj�ca adres w postaci binarnej uint16_t[8].
	 * \param [OUT] Tablica znak�w w kt�rej zostanie zapisany adres IP, minimum 40 elemet�w tablicy.
	 */
	void getShortAddress(uint16_t[], char*);

	/**
	 * Funkcja zwraca maksymaln� ilo�� host�w, kt�re mog� nale�e� do sieci przy zak�ddanej masce.
	 * 
	 * \return Maksymalna ilo�� host�w w sieci
	 */
	boost::multiprecision::uint128_t getMaxClients();

	/**
	 * 
	 * 
	 * \return d�ugo�� maski sieci w zapisie dziesi�tnym
	 */
	uint8_t getNetmask();
	/**
	 * Zwraca ci�gi znak�w zawieraj�cych adres IP sieci z wyznaczonymi r�nicami.
	 *
	 * \param [IN/OUT] Tablica znak�w w kt�rej zostanie zapisany adres IP, minimum 50 elemet�w tablicy.
	 * \param [IN/OUT] Tablica znak�w w kt�rej zostanie zapisany adres IP, minimum 50 elemet�w tablicy.
	 * \param [IN/OUT] Tablica znak�w w kt�rej zostanie zapisany adres IP, minimum 50 elemet�w tablicy.
	 */
	void diff3Address(char*, char*, char*);


};

