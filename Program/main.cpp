// Lab_12_11_1
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

struct Train {
	int number;
	char station[120];
	char time[120];

	Train* next = nullptr;
	Train* parent = nullptr;
};

void push_train(Train*& start, Train*& end, Train* ab) {
	if (start == nullptr) {
		end = start = ab;
		ab->next = nullptr;
		ab->parent = nullptr;
	}
	else {
		end->next = ab;
		ab->parent = end;
		ab->next = nullptr;
		end = ab;
	}
}

void train_factory(Train*& start, Train*& end) {
	char ch;
	cout << "Adding trains: " << endl;
	Train* ab;
	do
	{
		ab = new Train();
		cout << "Input number: "; cin >> ab->number;
		cin.ignore(INT_MAX, '\n');
		cout << "Input station: "; cin.getline(ab->station, 119);
		cout << "Input time of: "; cin.getline(ab->time, 119);
		push_train(start, end, ab);
		cout << "continue(y/n): "; cin >> ch;
	} while (ch == 'Y' || ch == 'y');
}

void draw_trains(Train* start) {
	cout << "==========================================" << endl;
	cout << "| Number         | Station        | Time |" << endl;
	cout << "------------------------------------------" << endl;
	while (start) {
		cout << "| " << setw(15) << start->number
			<< "| " << setw(15) << start->station
			<< "| " << setw(5) << start->time
			<< "|" << endl;
		start = start->next;
	}
	cout << "==========================================" << endl;
}

Train* get_train_by_number(Train* start, const int& number) {
	while (start) {
		if (start->number == number)
			break;
		start = start->next;
	}
	return start;
}

Train* get_all_trains_to_stations(Train* start, const string &station, Train* &out_s, Train*& out_e) {
	Train* tmp;
	while (start) {
		if (string(start->station) == station)
		{
			tmp = new Train();
			memcpy(tmp, start, sizeof(Train));
			push_train(out_s, out_e, tmp);
		}
		start = start->next;
	}
	return start;
}

void save_trains_to_file(fstream &f, Train* start) {
	f.clear();
	while (start) {
		f.write((char*)start, sizeof(Train) - sizeof(Train*) * 2);
		start = start->next;
	}
}

void load_trains_to_file(fstream& f, Train* &start, Train *&end) {
	f.clear();
	Train* tmp;
	while (!f.eof()) {
		tmp = new Train();
		f.read((char*)tmp, sizeof(Train) - sizeof(Train*) * 2);
		if (f.eof())
		{
			delete tmp;
			break;
		}
		push_train(start, end, tmp);
	}
}

void delete_trains(Train*& list, Train*& end) {
	if (list->next)
		delete_trains(list->next, end);
	else end = nullptr;
	delete list;
	list = nullptr;
}

void main() {
	int state = -1;
	string message;
	Train* trains[2] = {nullptr, nullptr};
	while (state != 0) {
		system("cls");
		draw_trains(trains[0]);
		if (message.size() > 0) {
			cout << message << endl;
			message.clear();
		}
		else cout << endl;
		cout << "[1] Add trains" << endl
			<< "[2] Get train info by number" << endl
			<< "[3] Get all trains by station" << endl
			<< "[4] Save trains to file" << endl
			<< "[5] Load trains from file" << endl
			<< "[0] Exit" << endl;
		cout << "Choose: ";
		do
		{
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
			cin >> state;
		} while (cin.fail());

		switch (state) {
		case 1:
			train_factory(trains[0], trains[1]);
			message = "Succesful add trains!";
			break;
		case 2: {
			cout << "Input number of train: ";
			int number;
			do
			{
				if (cin.fail())
				{
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}
				cin >> number;
			} while (cin.fail());
			Train *tmp = get_train_by_number(trains[0], number);
			if (tmp != nullptr)
				message = "Succesful find train ¹" + to_string(number) + '\n' +
				"Time: " + tmp->time + '\n' +
				"Station: " + tmp->station;
			else message = "Can't find train ¹" + to_string(number) + "!";

		}
			  break;
		case 3: {
			string station;
			Train* out[2] = {nullptr, nullptr};
			cout << "Input station: ";
			cin >> station;
			get_all_trains_to_stations(trains[0], station, out[0], out[1]);
			draw_trains(out[0]);
			delete_trains(out[0], out[1]);
			system("pause");
		}
			  break;
		case 4:
		{
			string filename;
			cout << "Input save filename: ";
			cin >> filename;
			fstream f(filename, ios::out);
			if (f.is_open()) {
				save_trains_to_file(f, trains[0]);
				f.close();
				message = "Scuccesful save to " + filename + "!";
			}else message = "Can't open " + filename + "!";
		}
		break;
		case 5:
		{
			string filename;
			cout << "Input load filename: ";
			cin >> filename;
			fstream f(filename, ios::in);
			if (f.is_open()) {
				load_trains_to_file(f, trains[0], trains[1]);
				f.close();
				message = "Scuccesful load from to " + filename + "!";
			}
			else message = "Can't open " + filename + "!";
		}
		default:
			message = "Incorrect item of menu!";
		}
	}
}