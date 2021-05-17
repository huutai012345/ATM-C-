#include<iostream>
#include<string>
#include<fstream>
//#include <chrono>
#include <ctime>
#pragma warning(disable:4996)
using namespace std;


struct User {
	string id;
	string pass;
	int soTien;
	bool trangThai = true;
};

struct DsUser {
	User* data[100];
	int soLg = 0;
};

struct Money {
	int soLg;
	int menhGia;
};

struct ATM {
	Money data[500];
	int soTien = 0;
	int soLg = 0;
};

struct LichSu {
	string id;
	int soTien;
	string date;
	string loaiGD;
};

struct DSLichSu {
	LichSu data[500];
	int soLg = 0;
};


void loadUser(DsUser& dsUser)
{
	ifstream fileIn;
	fileIn.open("dsNgDung.txt", ios::in);

	fileIn >> dsUser.soLg;
	fileIn.ignore();
	for (int i = 0; i < dsUser.soLg; i++)
	{
		User* user = new User;
		getline(fileIn, user->id, ',');
		getline(fileIn, user->pass, ',');
		fileIn >> user->soTien;
		fileIn.ignore();
		fileIn >> user->trangThai;
		fileIn.ignore();

		dsUser.data[i] = user;
	}
	fileIn.close();
}
void saveUser(DsUser dsUser)
{
	ofstream fileOut;
	fileOut.open("dsNgDung.txt", ios::out);

	fileOut << dsUser.soLg << endl;
	for (int i = 0; i < dsUser.soLg; i++)
	{
		fileOut << dsUser.data[i]->id << ',' << dsUser.data[i]->pass << "," << dsUser.data[i]->soTien << "," << dsUser.data[i]->trangThai << endl;
	}

	fileOut.close();
}

void themTienATM(ATM& atm, Money money)
{
	if (atm.soLg == 0)
	{
		atm.data[0] = money;
		atm.soLg++;
		atm.soTien += money.soLg * money.menhGia;
		return;
	}
	
	if (money.menhGia > atm.data[atm.soLg-1].menhGia)
	{
		atm.data[atm.soLg++] = money;
		atm.soTien += money.soLg * money.menhGia;
		return;
	}

	bool kt = false;
	for (int i = 0; i < atm.soLg; i++) {
		if (money.menhGia == atm.data[i].menhGia)
		{
			atm.data[i].soLg = +money.soLg;
			kt = true;
		}
	}

	if (!kt)
	{
		for (int i = 0; i < atm.soLg; i++)
		{
			if (money.menhGia < atm.data[i].menhGia)
			{
				for (int j = atm.soLg; j > i ; j--)
				{
					atm.data[j] = atm.data[j - 1];
				}

				atm.data[i] = money;
				atm.soLg++;
				break;
			}
		}
	}

	atm.soTien += money.soLg * money.menhGia;
}
void loadATM(ATM& atm)
{
	ifstream fileIn;
	fileIn.open("atm.txt", ios::in);

	int n;
	fileIn >> n;
	fileIn.ignore();
	for (int i = 0; i < n; i++)
	{
		Money money;

		fileIn >> money.menhGia;
		fileIn.ignore();
		fileIn >> money.soLg;
		fileIn.ignore();

		themTienATM(atm, money);
	}

	fileIn.close();
}
void saveATM(ATM atm)
{
	ofstream fileOut;
	fileOut.open("atm.txt", ios::out);

	fileOut << atm.soLg << endl;
	for (int i = 0; i < atm.soLg; i++)
	{
		fileOut << atm.data[i].menhGia << ',' << atm.data[i].soLg << endl;
	}

	fileOut.close();
}

void xuatDSNgDung(DsUser dsUser) {
	for (int i = 0; i < dsUser.soLg; i++)
	{
		cout << dsUser.data[i]->id << " " << dsUser.data[i]->pass << " " << dsUser.data[i]->soTien << " " << dsUser.data[i]->trangThai << endl;
	}
}
void xuatATM(ATM atm) {
	cout << "Tong So Tien: " << atm.soTien << endl;

	for (int i = 0; i < atm.soLg; i++)
	{
		cout << atm.data[i].menhGia << " " << atm.data[i].soLg << endl;
	}
}

User* getUser(DsUser dsUser, string id) {
	for (int i = 0; i < dsUser.soLg; i++)
	{
		if (dsUser.data[i]->id == id)
		{
			return dsUser.data[i];
		}
	}

	return NULL;
}
User* dangNhap(DsUser& dsUser)
{
	string id, pass;
	int i = 0;

	User* user = NULL;
	do {
		cout << "Nhap ID: ";
		getline(cin, id);
		if (id == "exit")
		{
			return NULL;
		}
		user = getUser(dsUser, id);
		if (user == NULL)
		{
			cout << "User Khong Ton Tai" << endl;
			cout << "Nhap Lai Hoac Go \"exit\" De Thoat"<<endl;
		}
	} while (user == NULL);
	

	if (user->trangThai == 0)
	{
		cout << "The Da Bi Khoa" << endl;
		return NULL;
	}

	while (i < 5)
	{
		cout << "Nhap pass: ";
		getline(cin, pass);

		if (user->pass == pass)
		{
			return user;
		}
		else {
			cout << "Sai mat khau" << endl;
		}

		i++;
	}

	cout << "The Da Bi Khoa" << endl;
	user->trangThai = 0;
	return NULL;
}
bool checkRutTien(int soTienMin, int soTien)
{
	while (soTien>0)
	{
		if (soTien < soTienMin)
		{
			return false;
		}

		soTien -= soTienMin;
	}

	return true;
}
void rutTien(ATM& atm,int soTien) {

	for (int i = atm.soLg - 1; i > 0; i--)
	{
		if (soTien == 0)
		{
			break;
		}

		if (atm.data[i].menhGia == soTien)
		{
			cout << "\tTo " << atm.data[i].menhGia << " x1"<<endl;

			soTien = 0;
			atm.data[i].soLg--;
			break;
		}

		if (atm.data[i].menhGia < soTien)
		{
			int soLgTien1MenhGia = atm.data[i].soLg;
			for (int j = 0; j < atm.data[i].soLg; j++)
			{
				soTien -= atm.data[i].menhGia;
				soLgTien1MenhGia--;

				if (soTien == 0 || soTien < atm.data[i].menhGia)
				{
					cout << "\tTo " << atm.data[i].menhGia<< " x"<< j+1<<endl;

					atm.data[i].soLg = soLgTien1MenhGia;
					break;
				}
			}
		}
	}
}

void themLichSu(DSLichSu &dsLS,string id,int soTien,string loaiGD)
{
	LichSu ls;
	ls.id = id;
	ls.soTien = soTien;
	
	ls.loaiGD = loaiGD;
	
    time_t now = time(0);
    tm *ltm = localtime(&now);

	ls.date = to_string(1900 + ltm->tm_year) + '/' + to_string(1 + ltm->tm_mon)
		+ '/' + to_string(ltm->tm_mday) + ' ' + to_string(5 + ltm->tm_hour) + ":" + to_string(30 + ltm->tm_min) + ":" + to_string(ltm->tm_sec);

	dsLS.data[dsLS.soLg++] = ls;
}
void loadLS(DSLichSu &dsLS) {
	ifstream fileIn;
	fileIn.open("lichsu.txt", ios::in);

	fileIn >> dsLS.soLg;
	fileIn.ignore();
	for (int i = 0; i < dsLS.soLg; i++)
	{
		LichSu ls;
		getline(fileIn, ls.id, ',');
		fileIn >> ls.soTien;
		fileIn.ignore();
		getline(fileIn, ls.loaiGD, ',');
		getline(fileIn, ls.date);

		dsLS.data[i] = ls;
	}
	fileIn.close();
}
void saveLS(DSLichSu dsLS) {
	ofstream fileOut;
	fileOut.open("lichsu.txt", ios::out);

	fileOut << dsLS.soLg << endl;
	for (int i = 0; i < dsLS.soLg; i++)
	{
		fileOut << dsLS.data[i].id << "," << dsLS.data[i].soTien <<","<< dsLS.data[i].loaiGD <<","<< dsLS.data[i].date << endl;
	}
	fileOut.close();
}

void menu()
{
	cout << "\t MENU" << endl;
	cout << "1. Gui Tien Vao ATM" << endl;
	cout << "2. Rut Tien" << endl;
	cout << "0. Thoat" << endl;
	cout << "Chon: " ;
}

int main()
{
	DSLichSu dsLS;
	DsUser dsUser;
	ATM atm;

	loadLS(dsLS);
	loadUser(dsUser);
	loadATM(atm);

	cout << "\n\t\tWELCOME TO ATM\n"<<endl;
	cout << "----------------------------------------------------------------------------------" << endl;
	User* user = dangNhap(dsUser);
	if (user == NULL)
	{
		cout << "Vui Long Nhan Lai The";
		system("pause");
		return 0;
	}

	int luaChon = 0;
	while (true)
	{
		system("cls");
		
		menu();
		cin >> luaChon;

		switch (luaChon)
		{
		case 1:
		{
			Money money;
			cout << "Nhap Menh Gia: ";
			cin >> money.menhGia;
			cout << "Nhap So Luong: ";
			cin >> money.soLg;

			user->soTien += money.soLg * money.menhGia;
			themTienATM(atm, money);

			themLichSu(dsLS, user->id, money.menhGia * money.soLg, "Nap Tien");
			cout << "Nap Tien Thanh Cong"<<endl;
			system("pause");
			break;
		}
		case 2:
		{
			cout << "So Tien Nho Nhat Dc Rut Ra: " << atm.data[0].menhGia<<endl;
			int soTien;
			cout << "Nhap So Tien Can Rut: ";
			cin >> soTien;

			if (soTien > user->soTien)
			{
				cout << "Ban Khong Du Tien De Rut" << endl;
			}
			else if (soTien < atm.data[0].menhGia)
			{
				cout << "Vui Long Nhap So Tien Lon Hon Menh Gia Nho Nhat" << endl;
			}
			else if (!checkRutTien(atm.data[0].menhGia,soTien))
			{
				cout << "So Tien Cua Ban Co Menh Gia Nho Hon Menh Gia Nho Nhat" << endl;
			}
			else {
				cout << "------------------------------------------" << endl;
				cout << "Menh Gia Tien Dc Rut Ra La: "<<endl;
				rutTien(atm, soTien);
				
				user->soTien -= soTien;
				cout << "Rut Tien Thanh Cong So Du Con Lai La: " << user->soTien<<endl;
				themLichSu(dsLS, user->id, soTien, "Rut Tien");
			}

			system("pause");
			break;
		}
		case 0:
		{
			saveLS(dsLS);
			saveUser(dsUser);
			saveATM(atm);
			cout << "Xin Cam On" << endl;
			system("pause");
			exit(0);
		}
		default:
			cout << "Input invalid";
			break;
		}
	}
	return 0;
}
