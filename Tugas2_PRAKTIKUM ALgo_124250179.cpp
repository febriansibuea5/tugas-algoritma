#include <iostream>
#include <string>
#include <queue>
#include <stack>
using namespace std;
struct Kendaraan {
    string nopol;
    string pemilik;
    string status;
    Kendaraan* kiri;
    Kendaraan* kanan;
    Kendaraan(string np, string p) : nopol(np), pemilik(p), status("Di Luar Parkiran"), kiri(nullptr), kanan(nullptr) {}
};
Kendaraan* root = nullptr;
int kapasitas = 5;
int isiParkir = 0;
queue<string> antrianParkir;
stack<pair<string,string>> histori;
Kendaraan* insert(Kendaraan* node, string nopol, string pemilik) {
    if (!node) return new Kendaraan(nopol, pemilik);
    if (nopol < node->nopol) node->kiri = insert(node->kiri, nopol, pemilik);
    else if (nopol > node->nopol) node->kanan = insert(node->kanan, nopol, pemilik);
    return node;
}
Kendaraan* cari(Kendaraan* node, string nopol) {
    if (!node || node->nopol == nopol) return node;
    if (nopol < node->nopol) return cari(node->kiri, nopol);
    return cari(node->kanan, nopol);
}
void tampilInorder(Kendaraan* node) {
    if (!node) return;
    tampilInorder(node->kiri);
    cout << "  " << node->nopol << " | " << node->pemilik << " | " << node->status << "\n";
    tampilInorder(node->kanan);
}
bool adaData(Kendaraan* node) {
    return node != nullptr;
}
Kendaraan* findMin(Kendaraan* node) {
    while (node->kiri) node = node->kiri;
    return node;
}
Kendaraan* hapus(Kendaraan* node, string nopol, bool& ditemukan) {
    if (!node) return nullptr;
    if (nopol < node->nopol) node->kiri = hapus(node->kiri, nopol, ditemukan);
    else if (nopol > node->nopol) node->kanan = hapus(node->kanan, nopol, ditemukan);
    else {
        ditemukan = true;
        if (!node->kiri) { Kendaraan* tmp = node->kanan; delete node; return tmp; }
        if (!node->kanan) { Kendaraan* tmp = node->kiri; delete node; return tmp; }
        Kendaraan* suksessor = findMin(node->kanan);
        node->nopol = suksessor->nopol;
        node->pemilik = suksessor->pemilik;
        node->status = suksessor->status;
        node->kanan = hapus(node->kanan, suksessor->nopol, ditemukan);
    }
    return node;
}
void tambahKendaraan() {
    string nopol, pemilik;
    cout << "Nomor Polisi : "; cin.ignore(); getline(cin, nopol);
    cout << "Nama Pemilik : "; getline(cin, pemilik);
    if (cari(root, nopol)) { cout << "Nomor polisi sudah terdaftar!\n"; return; }
    root = insert(root, nopol, pemilik);
    cout << "Data kendaraan berhasil ditambahkan.\n";
}
void tampilKendaraan() {
    if (!adaData(root)) { cout << "Belum ada data kendaraan.\n"; return; }
    cout << "\n  Nomor Polisi     | Nama Pemilik         | Status\n";
    cout << "  " << string(60, '-') << "\n";
    tampilInorder(root);
}
void kendaraanMasuk() {
    string nopol;
    cout << "Nomor Polisi : "; cin.ignore(); getline(cin, nopol);
    Kendaraan* k = cari(root, nopol);
    if (!k) { cout << "Kendaraan tidak ditemukan!\n"; return; }
    if (k->status == "Di Parkiran") { cout << "Kendaraan sudah berada di parkiran.\n"; return; }
    if (isiParkir < kapasitas) {
        k->status = "Di Parkiran";
        isiParkir++;
        histori.push({nopol, "masuk"});
        cout << "Kendaraan berhasil masuk.\n";
    } else {
        antrianParkir.push(nopol);
        cout << "Parkiran penuh, kendaraan masuk ke dalam antrian.\n";
    }
}
void kendaraanKeluar() {
    string nopol;
    cout << "Nomor Polisi : "; cin.ignore(); getline(cin, nopol);
    Kendaraan* k = cari(root, nopol);
    if (!k) { cout << "Kendaraan tidak ditemukan!\n"; return; }
    if (k->status != "Di Parkiran") { cout << "Kendaraan tidak sedang di parkiran.\n"; return; }
    k->status = "Di Luar Parkiran";
    isiParkir--;
    histori.push({nopol, "keluar"});
    cout << "Kendaraan berhasil keluar.\n";
    if (!antrianParkir.empty()) {
        string next = antrianParkir.front(); antrianParkir.pop();
        Kendaraan* kNext = cari(root, next);
        if (kNext) { kNext->status = "Di Parkiran"; isiParkir++; }
        cout << "Kendaraan berikutnya langsung masuk ke area parkir dari antrian.\n";
    }
}
void hapusKendaraan() {
    string nopol;
    cout << "Nomor Polisi : "; cin.ignore(); getline(cin, nopol);
    bool ditemukan = false;
    root = hapus(root, nopol, ditemukan);
    if (ditemukan) cout << "Data kendaraan berhasil dihapus.\n";
    else cout << "Kendaraan tidak ditemukan!\n";
}
void undoAksi() {
    if (histori.empty()) { cout << "Tidak ada aksi untuk di-undo.\n"; return; }
    auto [nopol, aksi] = histori.top(); histori.pop();
    Kendaraan* k = cari(root, nopol);
    if (!k) { cout << "Data kendaraan tidak ditemukan.\n"; return; }
    if (aksi == "masuk") {
        k->status = "Di Luar Parkiran";
        isiParkir--;
        cout << "Undo: Kendaraan keluar dari parkiran.\n";
    } else {
        k->status = "Di Parkiran";
        isiParkir++;
        cout << "Undo: Kendaraan masuk kembali ke parkiran.\n";
    }
}
void tampilAntrian() {
    if (antrianParkir.empty()) { cout << "Antrian kosong.\n"; return; }
    queue<string> tmp = antrianParkir;
    while (!tmp.empty()) {
        cout << "  " << tmp.front() << " menunggu giliran masuk parkiran.\n";
        tmp.pop();
    }
}
int main() {
    int pilihan;
    cout << "========================================\n";
    cout << "   SISTEM PARKIR INAP BANDARA GSA\n";
    cout << "========================================\n";
    do {
        cout << "\n[1] Tambah Kendaraan\n[2] Tampil Kendaraan\n[3] Kendaraan Masuk\n";
        cout << "[4] Kendaraan Keluar\n[5] Hapus Kendaraan\n[6] Undo\n[7] Tampil Antrian\n[0] Keluar\n";
        cout << "Pilihan : "; cin >> pilihan;
        switch(pilihan) {
            case 1: tambahKendaraan(); break;
            case 2: tampilKendaraan(); break;
            case 3: kendaraanMasuk(); break;
            case 4: kendaraanKeluar(); break;
            case 5: hapusKendaraan(); break;
            case 6: undoAksi(); break;
            case 7: tampilAntrian(); break;
            case 0: cout << "Sistem ditutup.\n"; break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while(pilihan != 0);
    return 0;
}
