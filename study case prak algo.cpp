#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
struct Buku {
    string nama;
    float harga;
    string daftarBuku;
};
void tampilkanBuku(const vector<Buku>& daftarBuku) {
    for (const auto& buku : daftarBuku) {
        cout << "Nama Buku: " << buku.nama << ", Harga: Rp " << buku.harga << endl;}}
int sequentialSearch(const vector<Buku>& daftarBuku, const string& namaBuku) {
    for (int i = 0; i < daftarBuku.size(); i++) {
        if (daftarBuku[i].nama == namaBuku) {
            return i;}}
    return -1;}
int binarySearch(const vector<Buku>& daftarBuku, const string& namaBuku) {
    int left = 0;
    int right = daftarBuku.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (daftarBuku[mid].nama == namaBuku) {
            return mid;}
        if (daftarBuku[mid].nama < namaBuku) {
            left = mid + 1;
        } else {
            right = mid - 1;}}
    return -1;}
int main() {
    vector<Buku> daftarBuku = {
        {"Buku A", 50000},
        {"Buku B", 75000},
        {"Buku C", 60000},
        {"Buku D", 45000},
        {"Buku E", 90000}
    };
    cout << "Daftar Buku Tersedia:\n";
    tampilkanBuku(daftarBuku);
    cout << "\nMasukkan nama buku yang ingin dicari: ";
    string namaBuku;
    cin >> namaBuku;
    cout << "\nPilih metode pencarian:\n";
    cout << "1. Sequential Search\n";
    cout << "2. Binary Search\n";
    cout << "Pilih metode pencarian (1/2): ";
    int pilihan;
    cin >> pilihan;
    int index;
    if (pilihan == 1) {
        index = sequentialSearch(daftarBuku, namaBuku);
        if (index != -1) {
            cout << "Buku ditemukan! Harga: Rp " << daftarBuku[index].harga << endl;
        } else {
            cout << "Buku tidak ditemukan.\n";
        }
    } else if (pilihan == 2) {
        sort(daftarBuku.begin(), daftarBuku.end(), [](const Buku& a, const Buku& b) {
            return a.nama < b.nama;
        });
        index = binarySearch(daftarBuku, namaBuku);
        if (index != -1) {
            cout << "Buku ditemukan! Harga: Rp " << daftarBuku[index].harga << endl;
        } else {
            cout << "Buku tidak ditemukan.\n";
        }
    } else {
        cout << "Pilihan tidak valid.\n";}
    return 0;
}
