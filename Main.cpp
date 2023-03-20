#include "stdio.h"
#include "DateTime.h"
#include "Objects.h"
#include "Headers.h"
#include "Structs.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "time.h"

#pragma warning ( disable : 4996 )

//Alamfunktsioonid
void PrintObjects(HeaderC* pStruct4) {

	//Viit, mis viitab esimesele header struktuurile
	HeaderC* pStructTemp = pStruct4;
	int i = 1, j = 1, k;

	//Teeb tsükklit nii kaua, kuni jõuab viimase struktuurini
	while (pStructTemp != NULL) {

		//Prindib headeri järjekorra numbri ja selle alguse karakteri
		printf("\nHeader %d: %c\n", i, pStructTemp->cBegin);

		//Ajutine dopelt viit, mis viitab viitade jada algusesse
		Object9** pPointerOriginal = (Object9**)pStructTemp->ppObjects;

		//Käib läbi kõik viidad viitade jadas
		for (k = 0; k < 26; ++k) {
			Object9* pPointerTemp = pPointerOriginal[k];

			//Käib kõik objektid läbi
			while (pPointerTemp != NULL) {

				//Andmete printimine
				Date3* pDateTemp = (Date3*)pPointerTemp->pDate3;
				printf("		%d) %s %lu %02d %s %04d\n", j, pPointerTemp->pID, pPointerTemp->Code, pDateTemp->Day, pDateTemp->pMonth, pDateTemp->Year);
				pPointerTemp = pPointerTemp->pNext;
				j++;

			}

		}

		//Viit saab uue väärtuse, mis viitab järgmisele header struktuurile
		pStructTemp = pStructTemp->pNext;
		i++;
	}

}

bool Letters(char* letter) {
	//Kontrollime, kas kõik elemendid on ainult tähed
	while (*letter != '\0') {
		if (isalpha(*letter) == 0) {
			if (*letter == ' ') {

			}
			else {
				return false;
			}
		}
		letter++;
	}
	return true;
}

bool lowerCase(char* letter) {
	letter++;

	while (*letter != '\0') {
		if (isspace(*letter) == 0) {
			if (isupper(*letter) != 0) {
				return false;
			}
		}
		else {
			letter++;
			if (isupper(*letter) == 0) {
				return false;
			}
		}
		letter++;
	}
	return true;
}

bool spaceCheck(char* letter) {
	letter++;

	for (; *letter != '\0'; letter++) {
		if (*letter == ' ') {
			return true;
		}
	}

	return false;
}

Object9* pObject9New(char* pNewID, unsigned long int NewCode, Object9* pNextOb) {

	//Anname mälu uuele objektile
	Object9* pObjectNew = (Object9*)malloc(sizeof(Object9));

	//Kood uuele objektile
	pObjectNew->Code = NewCode;

	//Kuupäeva sisestamine uuele objektile, 11 on tähtede arv kõige pikemas kuus
	time_t RawTime = time(&RawTime);
	char* pMonthBuf = (char*)malloc(11);
	Date3* pTime = (Date3*)malloc(sizeof(Date3));

	GetDate3(RawTime, 11, pMonthBuf, pTime);

	pObjectNew->pDate3 = pTime;

	//Anname mälu uuele ID-le
	pObjectNew->pID = (char*)malloc(strlen(pNewID));

	//Sisestame ID uude objekti
	strcpy(pObjectNew->pID, pNewID);

	//Sisestame viida, mis viitab järgnevale objektile
	pObjectNew->pNext = pNextOb;
	return pObjectNew;
}

HeaderC* pNewHeader(char* pNewID, HeaderC* pNext) {

	HeaderC* pNewStruct = (HeaderC*)malloc(sizeof(HeaderC));


	pNewStruct->pNext = pNext;
	pNewStruct->cBegin = *pNewID;



	void** pTemp = pNewStruct->ppObjects;
	pTemp = (void**)malloc(26 * sizeof(Object9**));

	for (int i = 0; i < 26; i++) {
		*pTemp = NULL;
		pTemp++;
	}
	pTemp -= 26;

	pNewStruct->ppObjects = pTemp;

	for (int i = 0; i < 26; i++) {
		*(pNewStruct->ppObjects) = NULL;
		(pNewStruct->ppObjects)++;

	}
	(pNewStruct->ppObjects) -= 26;
	return pNewStruct;
}

int InsertNewObject(HeaderC** pStruct4, char* pNewID, unsigned long int NewCode) {
	//Kontrollime, kas uus ID vastab formaadile
	if (!isupper(*pNewID) || !Letters(pNewID) || !lowerCase(pNewID) || !spaceCheck(pNewID)) {
		printf("\nVale formaat\n");
		return 0;
	}

	HeaderC* pStructTemp = *pStruct4;
	bool firstLoop = true;

	//Kontrollime, kas sellise ID-ga objekt ikka puudub
	while (pStructTemp != NULL) {
		//Kui ID esimene täht on võrdne headeri ID-ga
		if (*pNewID == pStructTemp->cBegin) {

			int k, i, j;
			Object9** pPointerOriginal = (Object9**)pStructTemp->ppObjects;

			//Käib läbi kõik viidad
			for (k = 0; k < 26; k++) {
				Object9* pPointerTemp = pPointerOriginal[k];
				//Käib läbi kõik objektid
				while (pPointerTemp != NULL) {


					if (strcmp(pNewID, pPointerTemp->pID) == 0) {
						printf("\nOn olemas juba sellise ID-ga\n");

						return 0;
					}

					//Järgmine objekt
					pPointerTemp = pPointerTemp->pNext;
				}
			}
			for (k = 0; k < 26; k++) {
				Object9* pPointerTemp = pPointerOriginal[k];

				while (pPointerTemp != NULL) {
					for (i = 0; *pNewID != ' '; pNewID++) {
						i++;
					}
					pNewID++;

					for (j = 0; *pPointerTemp->pID != ' '; pPointerTemp->pID++) {
						j++;
					}
					pPointerTemp->pID++;

					if (*pNewID == *pPointerTemp->pID) {
						pNewID -= i + 1;
						pPointerTemp->pID -= j + 1;
						pPointerTemp->pNext = pObject9New(pNewID, NewCode, pPointerTemp->pNext);
						printf("\nObjekt lisatud olemasolevasse objektide ahelasse: %s", pNewID);
						return 1;
					}
					pNewID -= i + 1;
					pPointerTemp->pID -= j + 1;
					pPointerTemp = pPointerTemp->pNext;
				}

			}

			for (i = 0; *pNewID != ' '; pNewID++) {
				i++;
			}
			pNewID++;

			if (*pNewID == 65) {
				pNewID -= i + 1;

				printf("\nObjekt lisatud olemasolevasse headerisse esimesse viita: %s", pNewID);
				*pPointerOriginal = pObject9New(pNewID, NewCode, NULL);
				break;
			}

			for (k = 0; k < 26; k++) {

				if (k == (*pNewID - 65)) {
					pNewID -= i + 1;

					*pPointerOriginal = pObject9New(pNewID, NewCode, NULL);
					printf("\nObjekt lisatud olemasolevasse headerisse: %s", pNewID);

					break;
				}
				pPointerOriginal++;
			}
			break;
		}
		//Kui Headerit pole sellise algus ID-ga nagu antud ID
		else if (pStructTemp->pNext == NULL) {

			//Uus header on viimane
			pStructTemp->pNext = pNewHeader(pNewID, pStructTemp->pNext);
			pStructTemp = pStructTemp->pNext;

			Object9** pPointerOriginal = (Object9**)pStructTemp->ppObjects;

			int k = 0, i;

			for (i = 0; *pNewID != ' '; pNewID++) {
				i++;
			}
			pNewID++;


			while (k < 26) {

				if (k == (*pNewID - 65)) {
					pNewID -= i + 1;

					*pPointerOriginal = pObject9New(pNewID, NewCode, NULL);
					printf("\nObjekt lisatud viimasesse uude headerisse: %s", pNewID);

					break;
				}
				pPointerOriginal++;
				k++;
			}

			break;
		}
		else if (pStructTemp->pNext->cBegin > *pNewID) {
			if (firstLoop == true) {
				//Uus header on esimene
				*pStruct4 = pNewHeader(pNewID, pStructTemp);
				pStructTemp = *pStruct4;

				Object9** pPointerOriginal = (Object9**)pStructTemp->ppObjects;

				int k = 0, i;

				for (i = 0; *pNewID != ' '; pNewID++) {
					i++;
				}
				pNewID++;


				while (k < 26) {

					if (k == (*pNewID - 65)) {
						pNewID -= i + 1;

						*pPointerOriginal = pObject9New(pNewID, NewCode, NULL);
						printf("\nObjekt lisatud esimesse uude headerisse: %s", pNewID);

						break;
					}
					pPointerOriginal++;
					k++;
				}

				break;
			}
			//Uus header on kuskil keskel
			pStructTemp->pNext = pNewHeader(pNewID, pStructTemp->pNext);
			pStructTemp = pStructTemp->pNext;

			Object9** pPointerOriginal = (Object9**)pStructTemp->ppObjects;

			int k = 0, i;

			for (i = 0; *pNewID != ' '; pNewID++) {
				i++;
			}
			pNewID++;


			while (k < 26) {

				if (k == (*pNewID - 65)) {
					pNewID -= i + 1;

					*pPointerOriginal = pObject9New(pNewID, NewCode, NULL);
					printf("\nObjekt lisatud keskmisesse uude headerisse: %s", pNewID);

					break;
				}
				pPointerOriginal++;
				k++;
			}

			break;
		}
		firstLoop = false;
		pStructTemp = pStructTemp->pNext;
	}
	return 1;
}

Object9* RemoveExistingObject(HeaderC** pStruct4, char* pExistingID) {

	HeaderC* pStructTempCheck = *pStruct4;
	bool firstLoop = true, esimeneH = false, viimaneH = false;
	int k, i, j, z = 0;

	while (pStructTempCheck != NULL) {
		if (*pExistingID == pStructTempCheck->cBegin) {
			if (*pExistingID == 65) {
				esimeneH = true;
			}
			else if (*pExistingID == 90) {
				viimaneH = true;
			}

			Object9** pPointerOriginal = (Object9**)pStructTempCheck->ppObjects;

			for (k = 0; k < 26; k++) {

				Object9* pPointerTemp = pPointerOriginal[k];

				while (pPointerTemp != NULL) {
					z++;
					pPointerTemp = pPointerTemp->pNext;
				}

			}

		}
		pStructTempCheck = pStructTempCheck->pNext;
	}

	HeaderC* pStructTemp = *pStruct4;
	HeaderC* pStructPrevious = pStructTemp;

	//Käib läbi kõik Headerid
	while (pStructTemp != NULL) {

		//Kui otsitava stringi esimene karakter on sama Headeri ID-ga
		if (*pExistingID == pStructTemp->cBegin) {

			Object9** pPointerOriginal = (Object9**)pStructTemp->ppObjects;
			Object9** pPointerPrevious = pPointerOriginal;

			//Käib läbi kõik viidad
			for (k = 0; k < 26; k++) {
				Object9* pPointerTemp = pPointerOriginal[k];
				Object9* pPointerPrevious = pPointerOriginal[k];

				//Käib läbi kõik objektid
				while (pPointerTemp != NULL) {

					//Stringide paigutamine teise sõna peale, et neid kontrollida
					for (i = 0; *pExistingID != ' '; pExistingID++) {
						i++;
					}
					pExistingID++;

					for (j = 0; *pPointerTemp->pID != ' '; pPointerTemp->pID++) {
						j++;
					}
					pPointerTemp->pID++;

					//Kui teise sõna esitähed on samad(samas objektis)
					if (*pExistingID == *pPointerTemp->pID) {
						pExistingID -= i + 1;
						pPointerTemp->pID -= j + 1;
						//Kui otsitav sõna klapib olemasolevaga
						if (strcmp(pExistingID, pPointerTemp->pID) == 0) {
							//Kui on esimene tsükkel
							if (firstLoop) {
								//Esimene, järgnevat pole
								if (pPointerTemp->pNext == NULL) {
									//Ainukene headeris
									if (z == 1) {
										//Esimene header
										if (esimeneH == true) {
											printf("\nEemaldatud objekt, mis on esimene ning ainukene esimeses headeris: %s", pExistingID);
											pPointerOriginal[k] = NULL;
											*pStruct4 = pStructTemp->pNext;
											return pPointerTemp;
										}
										//Viimane header
										else if (viimaneH == true) {
											printf("\nEemaldatud objekt, mis on esimene ning ainukene viimases headeris: %s", pExistingID);
											pPointerOriginal[k] = NULL;
											pStructPrevious->pNext = NULL;
											return pPointerTemp;
										}
										//Kuskil keskel header
										else {
											printf("\nEemaldatud objekt, mis on esimene ning ainukene keskmises headeris: %s", pExistingID);
											pPointerOriginal[k] = NULL;
											pStructPrevious->pNext = pStructTemp->pNext;
											return pPointerTemp;
										}
									}
									//Mitu tükki headeris
									else {
										printf("\nEemaldatud objekt, mis on esimene ning j2rgnev puudub ning, mis pole ainuke headeris: %s", pExistingID);
										pPointerOriginal[k] = NULL;
										return pPointerTemp;
									}
								}
								//Esimene, järgnev olemas
								else {
									printf("\nEemaldatud objekt, mis on esimene ning j2rgnev on olemas: %s", pExistingID);
									pPointerOriginal[k] = pPointerTemp->pNext;
									return pPointerTemp;
								}
							}
							else {
								//Pole Esimene, järgnevat pole
								printf("\nEemaldatud objekt, mis pole esimene ning j2rgnev puudub: %s", pExistingID);
								pPointerPrevious->pNext = pPointerTemp->pNext;
								return pPointerTemp;
							}

						}

						//Stringid uuesti viimase sõna peale, kuna all pool viiakse nkn tagasi õigele kohale
						pExistingID += i + 1;
						pPointerTemp->pID += j + 1;
						firstLoop = false;
					}

					//Stringid tagasi algusesse
					pExistingID -= i + 1;
					pPointerTemp->pID -= j + 1;

					//Järgmine objekt, eelmise objekti jätame meelde
					pPointerPrevious = pPointerTemp;
					pPointerTemp = pPointerTemp->pNext;
				}

				//Resettime esimese tsükli kontrollija
				firstLoop = true;
			}
		}

		//Järgmine Header, eelmise Headeri jätame meelde
		pStructPrevious = pStructTemp;
		pStructTemp = pStructTemp->pNext;
	}

	//Otsitavat objekti ei leitud
	printf("\nSellise ID-ga Objekt puudub: %s\n", pExistingID);
	return 0;
}

Node* InsertNode(Node* pTree, void* pNewRecord) {

	//Uue tipu loomine
	Node* pNew = (Node*)malloc(sizeof(Node));
	pNew->pObject = pNewRecord;
	pNew->pLeft = pNew->pRight = 0;

	if (!pTree) {
		printf("Inserted as first node: %s %d\n", ((Object9*)pNew->pObject)->pID, ((Object9*)pNew->pObject)->Code);
		return pNew;	//Puu oli tühi
	}

	for (Node* p = pTree; 1; ) {
		if (((Object9*)pNew->pObject)->Code < ((Object9*)p->pObject)->Code) {
			printf("\nComparing new: %s %d\n", ((Object9*)pNew->pObject)->pID, ((Object9*)pNew->pObject)->Code);
			printf("with the old : %s %d\n", ((Object9*)p->pObject)->pID, ((Object9*)p->pObject)->Code);
			//Leidsime tühja koha
			if (!p->pLeft) {
				p->pLeft = pNew;
				printf("Inserted to left: %s %d\n", ((Object9*)pNew->pObject)->pID, ((Object9*)pNew->pObject)->Code);
				return pTree;
			}
			//Liigume vasakule
			else {
				printf("\nMoving once more to the left: %s %d\n", ((Object9*)pNew->pObject)->pID, ((Object9*)pNew->pObject)->Code);
				p = p->pLeft;
			}
		}
		else {
			printf("\nComparing new: %s %d\n", ((Object9*)pNew->pObject)->pID, ((Object9*)pNew->pObject)->Code);
			printf("with the old : %s %d\n", ((Object9*)p->pObject)->pID, ((Object9*)p->pObject)->Code);
			//Leidsime tühja koha
			if (!p->pRight) {
				p->pRight = pNew;
				printf("Inserted to right: %s %d\n", ((Object9*)pNew->pObject)->pID, ((Object9*)pNew->pObject)->Code);
				return pTree;
			}
			//Liigume paremale
			else {
				printf("\nMoving once more to the right: %s %d\n", ((Object9*)pNew->pObject)->pID, ((Object9*)pNew->pObject)->Code);
				p = p->pRight;
			}
		}
	}
}

Node* CreateBinaryTree(HeaderC* pStruct4) {

	Node* pBinaryTree = NULL;
	HeaderC* pStructTemp = pStruct4;

	//Käib läbi kõik headerid
	while (pStructTemp != NULL) {

		Object9** pPointerOriginal = (Object9**)pStructTemp->ppObjects;

		//Käib läbi kõik viidad
		for (int k = 0; k < 26; k++) {

			Object9* pPointerTemp = pPointerOriginal[k];

			//Käib läbi kõik objektid
			while (pPointerTemp != NULL) {

				pBinaryTree = InsertNode(pBinaryTree, pPointerTemp);
				pPointerTemp = pPointerTemp->pNext;
			}
		}
		pStructTemp = pStructTemp->pNext;
	}

	return pBinaryTree;
}

Stack* Push(Stack* pStack, void* pRecord)
{
	Stack* pNew;
	if (!pRecord)
	{
		return pStack;
	}
	pNew = (Stack*)malloc(sizeof(Stack));
	pNew->pObject = pRecord;
	pNew->pNext = pStack;
	return pNew;
}

Stack* Pop(Stack* pStack, void** pResult)
{
	Stack* p;
	if (!pStack)
	{
		*pResult = 0;
		return pStack;
	}
	*pResult = (Object9*)pStack->pObject;
	p = pStack->pNext;
	free(pStack);
	return p;
}

void TreeTraversal(Node* pTree) {

	printf("\n");

	int i = 1;
	Stack* pStack = 0;
	Node* p1 = pTree, * p2;
	if (!pTree)
		return;
	do {
		while (p1) {
			pStack = Push(pStack, p1);
			p1 = p1->pLeft;
		}
		pStack = Pop(pStack, (void**)&p2);
		printf("Node %d: ", i);
		i++;
		printf("%s %d\n", ((Object9*)p2->pObject)->pID, ((Object9*)p2->pObject)->Code);
		p1 = p2->pRight;
	} while (!(!pStack && !p1));
}

Node* DeleteTreeNodeNone(Node* pRoot, Node* pOriginal, Node* pTemp) {

	//Peab kustutama juurtipu
	if (pOriginal == NULL) {
		pRoot = NULL;
	}
	//Peab kustutama vasaku tipu
	else if (pTemp == pOriginal->pLeft) {
		pOriginal->pLeft = NULL;
	}
	//Peab kustutama parema tipu
	else {
		pOriginal->pRight = NULL;
	}

	free(pTemp);
	return pRoot;
}

Node* DeleteTreeNodeOne(Node* pRoot, Node* pOriginal, Node* pTemp) {
	Node *child;

	//Vasak tipp on olemas
	if (pTemp->pLeft != NULL) {
		child = pTemp->pLeft;
	}
	//Parem tipp on olemas
	else {
		child = pTemp->pRight;
	}

	//Tipp on juurtipp
	if (pOriginal == NULL) {
		pRoot = child;
	}
	//Tipp on vasak tipp
	else if (pTemp == pOriginal->pLeft) {
		pOriginal->pLeft = child;
	}
	//Tipp on parem tipp
	else {
		pOriginal->pRight = child;
	}

	free(pTemp);
	return pRoot;
}

Node* DeleteTreeNodeBoth(Node* pRoot, Node* pOriginal, Node* pTemp) {
	Node *newNode, *newParentNode;

	newParentNode = pTemp;
	newNode = pTemp->pRight;

	//Kui vasak tipp viitab mingile tipule
	while (newNode->pLeft != NULL) {
		newParentNode = newNode;
		newNode = newNode->pLeft;
	}

	pTemp->pObject = newNode->pObject;

	//Kui vasak tipp ja parem tipp on tühjad
	if (newNode->pLeft == NULL && newNode->pRight == NULL) {
		pRoot = DeleteTreeNodeNone(pRoot, newParentNode, newNode);
	}
	//Kui, kas vasakus või paremas tipus on andmeid
	else {
		pRoot = DeleteTreeNodeOne(pRoot, newParentNode, newNode);
	}
	return pRoot;
}

Node* DeleteTreeNode(Node *pTree, unsigned long int Code) {
	Node *pOriginal, *pTemp;

	pTemp = pTree;
	pOriginal = NULL;

	//Kontrollime kuniks jõuame viimase tipuni
	while (pTemp != NULL) {

		//Kui kood, mida kustutada tahame on sama, mis tipu kood(tipp on leitud)
		if (Code == ((Object9*)pTemp->pObject)->Code) {
			break;
		}
		//Salvestame tipu asukoha
		pOriginal = pTemp;
		//Kui kood, mida otsime on väiksem tipu koodist, siis liigume vasakule
		if (Code < ((Object9*)pTemp->pObject)->Code) {
			pTemp = pTemp->pLeft;
		}
		//Kui kood, mida otsime on suurem tipu koodist, siis liigume paremale
		else {
			pTemp = pTemp->pRight;
		}
	}

	//Koodi ei leitud
	if (pTemp == NULL) {
		printf("\nKoodi ei leitud sellest puust. Kood: %d\n", Code);
	}
	//Kui leitud koodiga tipul on 2 tippu, millele ta viitab
	else if (pTemp->pLeft != NULL && pTemp->pRight != NULL) {
		pTree = DeleteTreeNodeBoth(pTree, pOriginal, pTemp);
	}
	//Kui on ainult vasak tipp
	else if (pTemp->pLeft != NULL) {
		pTree = DeleteTreeNodeOne(pTree, pOriginal, pTemp);
	}
	//Kui on ainult parem tipp
	else if (pTemp->pRight != NULL) {
		pTree = DeleteTreeNodeOne(pTree, pOriginal, pTemp);
	}
	//Kui ei ole tippe
	else {
		pTree = DeleteTreeNodeNone(pTree, pOriginal, pTemp);
	}

	return pTree;
}

int main()
{
	// Kirjutage lähtestruktuuri genereeriv lause. See on:
	// d) Struct4 puhul:
	HeaderC* pStruct4 = GetStruct4(9, 35);
	// O asemel kirjutage juhendaja poolt antud objekti indeks.
	// N asemel kirjutage juhendaja poolt antud objektide arv.
	// N ei ole kunagi 0
	/*
	//Esimene funktsioon
	PrintObjects(pStruct4);

	//Teine funktsioon
	HeaderC** ppStruct4 = &pStruct4;
	const char* addIDd[] = { "Dx Gz","Dx Ga","Db Aa","Dk Za","Dr Wa","Aa Aa","Ab Ba","Za Xa","Za Ab","Za Ca","Wx Xa","Wx Aa","zb Kk","Zc ca","Dr Wa","ZB kk","Fa","Fa_Fa" };
	unsigned long int NewCode = 12345678;
	for (int i = 0; i < 18; i++) {
		InsertNewObject(ppStruct4, (char*)addIDd[i], NewCode);
	}
	//Kontroll printimine
	PrintObjects(pStruct4);

	//Kolmas funktsioon
	Object9* pObjectRemove;
	const char* removeIDd[] = { "Dx Gz","Dx Ga","Db Aa","Dk Za","Dr Wa","Aa Aa","Ab Ba","Za Xa","Za Ab","Za Ca","Wx Xa","Wx Aa","zb Kk","Zc ca","Dr Wa","ZB kk","Fa","Fa_Fa" };
	for (int i = 0; i < 18; i++) {
		pObjectRemove = RemoveExistingObject(ppStruct4, (char*)removeIDd[i]);
	}
	//Kontroll printimine
	PrintObjects(pStruct4);
	*/
	//Neljas funktsioon, N = 35
	pStruct4 = GetStruct4(9, 35);

	//Väljastab lähtestruktuur
	PrintObjects(pStruct4);
	printf("\n");

	//Moodustab kahendpuu
	Node* pBinaryTree35 = CreateBinaryTree(pStruct4);

	//Viies funktsioon, väljastab tipud ja tippude juurde kuuluvad objektid
	TreeTraversal(pBinaryTree35);

	//Kuues funktsioon, eemaldab puu juurtipu
	unsigned long int Code = 316985719;
	DeleteTreeNode(pBinaryTree35, Code);

	//Väljastab uue puu tipud ja tippude juurde kuuluvad objektid
	TreeTraversal(pBinaryTree35);

	//N = 10
	pStruct4 = GetStruct4(9, 10);

	//Väljastab lähtestruktuuri
	PrintObjects(pStruct4);
	printf("\n");

	//Moodustab kahendpuu
	Node* pBinaryTree10 = CreateBinaryTree(pStruct4);

	//Väljastab tipud ja tippude juurde kuuluvad objektid
	TreeTraversal(pBinaryTree10);

	//Eemaldab ette antud võtmega tipud
	Code = 258186307;
	DeleteTreeNode(pBinaryTree10, Code);
	
	//Väljastab uue puu tipud ja tippude juurde kuuluvad objektid
	TreeTraversal(pBinaryTree10);

	//Eemaldab tipu, mida ei ole olemas
	Code = 12345;
	DeleteTreeNode(pBinaryTree10, Code);

	return 0;
}

