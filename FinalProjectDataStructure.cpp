#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

// Definisi Struktur Buku
struct Buku {
    int id;
    string judul;
    string pengarang;
    string kategori;
    int tahun;
    bool diPinjam;
    string namaPeminjam; // New field for borrower's name
    string NPMPeminjam;  // New field for borrower's npm
    Buku* next;
};

// Definisi Node untuk Single Linked List
struct Node {
    Buku data;
    Node* next;
};

// Definisi Node untuk Binary Search Tree
struct TreeNode {
    Buku data;
    TreeNode* left;
    TreeNode* right;
};

// Definisi Node untuk Binary Search Tree Kategori
struct KategoriNode {
    string kategori;
    Node* headDaftarBuku;
    KategoriNode* left;
    KategoriNode* right;
};

// Definisi Struktur Queue untuk Peminjaman Buku
struct QueueNode {
    int idBuku;
    string judulBuku;
    string namaPeminjam;
    string NPMPeminjam;
    bool meminjam;
    QueueNode* next;
};

// Variabel global untuk head dan tail dari queue
QueueNode* queueFront = nullptr;
QueueNode* queueRear = nullptr;


Node* head = nullptr;
TreeNode* root = nullptr;
KategoriNode* kategoriRoot = nullptr;

// Fungsi untuk Menambah Buku ke Linked List
void tambahBukuKeDaftar(const Buku& bukuBaru) {
    Node* newNode = new Node;
    newNode->data = bukuBaru;
    newNode->next = head;
    head = newNode;
}

// Fungsi untuk Menambah Buku ke Binary Search Tree berdasarkan tahun terbit
void tambahBukuKeTree(const Buku& bukuBaru, TreeNode*& node) {
    if (node == nullptr) {
        node = new TreeNode;
        node->data = bukuBaru;
        node->left = nullptr;
        node->right = nullptr;
    }
    else if (bukuBaru.tahun < node->data.tahun) {
        tambahBukuKeTree(bukuBaru, node->left);
    }
    else {
        tambahBukuKeTree(bukuBaru, node->right);
    }
}

// Fungsi untuk Menambah Buku ke Binary Search Tree Kategori
void tambahBukuKeKategoriTree(const Buku& bukuBaru, KategoriNode*& node) {
    if (node == nullptr) {
        node = new KategoriNode;
        node->kategori = bukuBaru.kategori;
        node->headDaftarBuku = nullptr;
        node->left = nullptr;
        node->right = nullptr;
    }

    // Jika kategori sudah ada, tambahkan buku ke dalam linked list kategori tersebut
    if (bukuBaru.kategori == node->kategori) {
        Node* newNode = new Node;
        newNode->data = bukuBaru;
        newNode->next = node->headDaftarBuku;
        node->headDaftarBuku = newNode;
    }
    else if (bukuBaru.kategori < node->kategori) {
        tambahBukuKeKategoriTree(bukuBaru, node->left);
    }
    else {
        tambahBukuKeKategoriTree(bukuBaru, node->right);
    }
}

// Fungsi untuk Mencari Buku Berdasarkan Kategori
void mencariBukuBerdasarkanKategori(KategoriNode* node, const string& kategori, bool& ditemukan) {
    if (node != nullptr && !ditemukan) {
        if (kategori == node->kategori) {
            cout << "Buku dalam kategori '" << kategori << "':" << endl;
            Node* current = node->headDaftarBuku;
            while (current != nullptr) {
                cout << "ID: " << current->data.id << ", Judul: " << current->data.judul
                    << ", Pengarang: " << current->data.pengarang << ", Tahun Terbit: " << current->data.tahun << endl;
                current = current->next;
            }
            cout << "---------------------------" << endl;
            ditemukan = true;
            return;  // Keluar dari fungsi setelah menemukan buku
        }
        mencariBukuBerdasarkanKategori(node->left, kategori, ditemukan);
        mencariBukuBerdasarkanKategori(node->right, kategori, ditemukan);
    }
}


// Fungsi untuk Mencari Buku Berdasarkan Tahun Terbit
void mencariBukuBerdasarkanTahun(TreeNode* node, int tahun, bool& ditemukan) {
    if (node != nullptr) {
        if (tahun == node->data.tahun) {
            cout << "ID: " << node->data.id << ", Judul: " << node->data.judul
                << ", Pengarang: " << node->data.pengarang << ", Kategori: " << node->data.kategori << ", Tahun Terbit: " << node->data.tahun << endl;
            ditemukan = true;
            // Tidak menggunakan return di sini agar pencarian bisa melanjutkan ke buku lainnya
        }
        mencariBukuBerdasarkanTahun(node->left, tahun, ditemukan);
        mencariBukuBerdasarkanTahun(node->right, tahun, ditemukan);
    }
}

// Fungsi untuk Mencari Buku Berdasarkan Judul
void mencariBukuBerdasarkanJudul() {
    string judul;
    cout << "Masukkan judul buku yang dicari: ";
    cin.ignore();
    getline(cin, judul);

    Node* current = head;
    bool ditemukan = false;

    while (current != nullptr) {
        if (current->data.judul == judul) {
            ditemukan = true;
            cout << "ID: " << current->data.id << ", Judul: " << current->data.judul
                << ", Pengarang: " << current->data.pengarang << ", Kategori: " << current->data.kategori << ", Tahun Terbit: " << current->data.tahun << endl;
        }
        current = current->next;
    }

    if (!ditemukan) {
        cout << "Buku dengan judul '" << judul << "' tidak ditemukan." << endl;
    }
}

// Fungsi untuk Menampilkan Semua Buku dalam Daftar
void menampilkanDaftarBuku() {
    cout << "======== Daftar Buku ========" << endl;
    Node* current = head;
    while (current != nullptr) {
        cout << "ID: " << current->data.id << ", Judul: " << current->data.judul
            << ", Pengarang: " << current->data.pengarang << ", Kategori: " << current->data.kategori << ", Tahun Terbit: " << current->data.tahun << endl;
        current = current->next;
    }
    cout << "=============================" << endl;
}

// Fungsi untuk Menampilkan Daftar Peminjaman dari Queue
void menampilkanQueue() {
    cout << "======== Daftar Antrian Peminjaman ========" << endl;

    QueueNode* current = queueFront;
    while (current != nullptr) {
        cout << "ID Buku: " << current->idBuku << ", Nama Peminjam: " << current->namaPeminjam
            << ", NPM Peminjam: " << current->NPMPeminjam << ", Status: ";
        if (current->meminjam) {
            cout << "Mengantri untuk meminjam";
        }
        else {
            cout << "Mengantri untuk mengembalikan";
        }
        cout << endl;

        current = current->next;
    }

    cout << "=========================================" << endl;
}

// Fungsi untuk Menambah Peminjaman ke Queue
void enqueuePinjam() {
    string mencariJudul;
    cout << "Cari judul buku yang ingin dipinjam: ";
    cin.ignore();
    getline(cin, mencariJudul);

    Node* current = head;
    bool ditemukan = false;

    while (current != nullptr) {
        if (current->data.judul == mencariJudul && !current->data.diPinjam) {
            ditemukan = true;
            // Update book information
            current->data.diPinjam = true;

            // Borrower information
            cout << "Nama peminjam: ";
            cin.ignore();
            getline(cin, current->data.namaPeminjam);

            cout << "NPM peminjam: ";
            getline(cin, current->data.NPMPeminjam);
            break;
        }
        current = current->next;
    }

    if (!ditemukan) {
        cout << "Buku berjudul \"" << mencariJudul << "\" sedang dipinjam." << endl;
        // Ask if the user wants to be in the queue
        char queuePilihan;
        cout << "Apakah ingin masuk antrian peminjam? (y/n): ";
        cin >> queuePilihan;
        if (queuePilihan == 'y' || queuePilihan == 'Y') {
            string namaPeminjam, NPMPeminjam;
            cout << "Masukkan Nama Peminjam: ";
            cin.ignore();
            getline(cin, namaPeminjam);

            cout << "Masukkan NPM Peminjam: ";
            getline(cin, NPMPeminjam);

            // Tambahkan peminjaman ke dalam queue
            QueueNode* newNode = new QueueNode;
            newNode->judulBuku = mencariJudul;
            newNode->namaPeminjam = namaPeminjam;
            newNode->NPMPeminjam = NPMPeminjam;
            newNode->meminjam = true;
            newNode->next = nullptr;

            if (queueFront == nullptr) {
                queueFront = newNode;
                queueRear = newNode;
            }
            else {
                queueRear->next = newNode;
                queueRear = newNode;
            }

            cout << "Anda telah masuk ke dalam antrian." << endl;
            menampilkanQueue();
        }
    }
    else {
        cout << "Berhasil meminjam buku" << endl;
    }
}

void enqueueKembali() {
    int idBuku;
    cout << "Masukkan ID Buku yang akan dikembalikan: ";
    cin >> idBuku;

    Node* current = head;
    bool ditemukan = false;

    while (current != nullptr) {
        if (current->data.id == idBuku && current->data.diPinjam) {
            ditemukan = true;
            // Update informasi buku
            current->data.diPinjam = false;
            current->data.namaPeminjam = "";
            current->data.NPMPeminjam = "";

            // Jika ada yang menunggu di antrian, pinjamkan ke orang pertama di antrian
            if (queueFront != nullptr) {
                // Ambil informasi peminjam dari antrian
                QueueNode* nodeDiPinjam = queueFront;
                // Update informasi buku
                current->data.diPinjam = true;
                current->data.namaPeminjam = nodeDiPinjam->namaPeminjam;
                current->data.NPMPeminjam = nodeDiPinjam->NPMPeminjam;
                // Hapus dari antrian
                queueFront = queueFront->next;
                delete nodeDiPinjam;

                cout << "Buku berhasil dikembalikan dan dipinjamkan ke " << current->data.namaPeminjam << " (NPM: " << current->data.NPMPeminjam << ")." << endl;
            }
            else {
                cout << "Buku berhasil dikembalikan dan kembali tersedia." << endl;
            }
            break;
        }
        current = current->next;
    }

    if (!ditemukan) {
        cout << "Buku tidak valid untuk dikembalikan." << endl;
    }
}

void menampilkanBukuYangDipinjam() {
    cout << "======== Daftar Buku Dipinjam ========" << endl;

    Node* current = head;
    while (current != nullptr) {
        if (current->data.diPinjam) {
            cout << "ID: " << current->data.id << ", Judul: " << current->data.judul
                << ", Pengarang: " << current->data.pengarang << ", Kategori: " << current->data.kategori
                << ", Tahun Terbit: " << current->data.tahun << ", Status Peminjaman: Dipinjam oleh "
                << current->data.namaPeminjam << " (NPM: " << current->data.NPMPeminjam << ")" << endl;
        }
        current = current->next;
    }

    cout << "=====================================" << endl;
}

// Fungsi untuk Hapus Buku
void hapusBuku() {
    int idBuku;
    cout << "Masukkan ID Buku yang akan dihapus: ";
    cin >> idBuku;

    Node* current = head;
    Node* prev = nullptr;
    bool ditemukan = false;

    while (current != nullptr) {
        if (current->data.id == idBuku) {
            ditemukan = true;
            // Jika buku yang dihapus berada di awal linked list
            if (prev == nullptr) {
                head = current->next;
            }
            else {
                prev->next = current->next;
            }

            delete current;
            cout << "Buku dengan ID " << idBuku << " berhasil dihapus." << endl;
            break;
        }
        prev = current;
        current = current->next;
    }

    if (!ditemukan) {
        cout << "Buku dengan ID " << idBuku << " tidak ditemukan." << endl;
    }
}

// Fungsi untuk Menambah Buku
void tambahBuku() {
    Buku bukuBaru;
    cout << "Masukkan ID Buku: ";
    cin >> bukuBaru.id;
    cout << "Masukkan Judul Buku: ";
    cin.ignore();
    getline(cin, bukuBaru.judul);
    cout << "Masukkan Pengarang Buku: ";
    getline(cin, bukuBaru.pengarang);
    cout << "Masukkan Kategori Buku: ";
    getline(cin, bukuBaru.kategori);
    cout << "Masukkan Tahun Terbit Buku: ";
    cin >> bukuBaru.tahun;
    bukuBaru.diPinjam = false;

    // Tambahkan buku ke Linked List
    tambahBukuKeDaftar(bukuBaru);

    // Tambahkan buku ke Binary Search Tree
    tambahBukuKeTree(bukuBaru, root);

    // Tambahkan buku ke Binary Search Tree Kategori
    tambahBukuKeKategoriTree(bukuBaru, kategoriRoot);

    cout << "Buku berhasil ditambahkan!" << endl;
}

// Fungsi untuk Menampilkan Halaman Pencarian
void halamanCari() {
    int pilihan;
    bool ditemukanTahun = false;
    bool ditemukanKategori = false;

    cout << "======== Halaman Cari ========\n";
    cout << "1. Cari berdasarkan Judul\n";
    cout << "2. Cari berdasarkan Tahun Terbit\n";
    cout << "3. Cari berdasarkan Kategori\n";
    cout << "============================\n";

    cout << "Masukkan pilihan Anda: ";
    cin >> pilihan;

    switch (pilihan) {
    case 1:
        mencariBukuBerdasarkanJudul();
        break;
    case 2:
        int mencariTahun;
        cout << "Masukkan tahun terbit buku yang dicari: ";
        cin >> mencariTahun;
        mencariBukuBerdasarkanTahun(root, mencariTahun, ditemukanTahun);
        if (!ditemukanTahun) {
            cout << "Data tidak ditemukan" << endl;
        }
        break;
    case 3:
        string mencariKategori;
        cout << "Masukkan kategori buku yang dicari: ";
        cin.ignore();
        getline(cin, mencariKategori);
        mencariBukuBerdasarkanKategori(kategoriRoot, mencariKategori, ditemukanKategori);
        if (!ditemukanKategori) {
            cout << "Data tidak ditemukan" << endl;
        }
        break;
    }
}

void daftarpeminjamanbuku() {
    cout << "======== Daftar Buku Dipinjam ========" << endl;

    Node* current = head;
    while (current != nullptr) {
        if (current->data.diPinjam) {
            cout << "ID: " << current->data.id << ", Judul: " << current->data.judul
                << ", Pengarang: " << current->data.pengarang << ", Kategori: " << current->data.kategori
                << ", Tahun Terbit: " << current->data.tahun << ", Status Peminjaman: Dipinjam" << endl;
        }
        current = current->next;
    }

    cout << "=====================================" << endl;
}

int main() {

    Buku buku1  = { 111, "Laskar Pelangi", "Andrea Hirata", "Fiksi", 2005, false, "", "", nullptr };
    Buku buku2  = { 112, "Bumi Manusia", "Pramoedya Ananta Toer", "Sastra", 1980, false, "", "", nullptr };
    Buku buku3  = { 113, "Arok-Dedes", "Pramoedya Ananta Toer", "Sejarah", 1999, false, "", "", nullptr };
    Buku buku4  = { 114, "Pulang", "Tere Liye", "Fiksi", 2016, false, "", "", nullptr };
    Buku buku5  = { 115, "Bidadari Bidadari Surga", "Tere Liye", "Fiksi", 2005, false, "", "", nullptr };
    Buku buku6  = { 116, "Perahu Kertas", "Dee Lestari", "Fiksi", 2008, false, "", "", nullptr };
    Buku buku7  = { 117, "Bumi", "Tere Liye", "Dokumenter", 2013, false, "", "", nullptr };
    Buku buku8  = { 118, "Hantu Jeruk Purut", "Richard Oh", "Horor", 2004, false, "", "", nullptr };
    Buku buku9  = { 119, "Rindu", "Tere Liye", "Romantis", 2009, false, "", "", nullptr };
    Buku buku10 = { 120, "Jejak Langkah", "Pramoedya Ananta Toer", "Sejarah", 1985, false, "", "", nullptr };

    tambahBukuKeDaftar(buku1);
    tambahBukuKeDaftar(buku2);
    tambahBukuKeDaftar(buku3);
    tambahBukuKeDaftar(buku4);
    tambahBukuKeDaftar(buku5);
    tambahBukuKeDaftar(buku6);
    tambahBukuKeDaftar(buku7);
    tambahBukuKeDaftar(buku8);
    tambahBukuKeDaftar(buku9);
    tambahBukuKeDaftar(buku10);

    tambahBukuKeTree(buku1, root);
    tambahBukuKeTree(buku2, root);
    tambahBukuKeTree(buku3, root);
    tambahBukuKeTree(buku4, root);
    tambahBukuKeTree(buku5, root);
    tambahBukuKeTree(buku6, root);
    tambahBukuKeTree(buku7, root);
    tambahBukuKeTree(buku8, root);
    tambahBukuKeTree(buku9, root);
    tambahBukuKeTree(buku10, root);

    tambahBukuKeKategoriTree(buku1, kategoriRoot);
    tambahBukuKeKategoriTree(buku2, kategoriRoot);
    tambahBukuKeKategoriTree(buku3, kategoriRoot);
    tambahBukuKeKategoriTree(buku4, kategoriRoot);
    tambahBukuKeKategoriTree(buku5, kategoriRoot);
    tambahBukuKeKategoriTree(buku6, kategoriRoot);
    tambahBukuKeKategoriTree(buku7, kategoriRoot);
    tambahBukuKeKategoriTree(buku8, kategoriRoot);
    tambahBukuKeKategoriTree(buku9, kategoriRoot);
    tambahBukuKeKategoriTree(buku10, kategoriRoot);

    int menu;

    do {
        cout << "======== Menu Utama ========" << endl;
        cout << "1. Tambah Buku" << endl;
        cout << "2. Hapus Buku" << endl;
        cout << "3. Cari Buku" << endl;
        cout << "4. Pinjam Buku" << endl;
        cout << "5. Kembalikan Buku" << endl;
        cout << "6. Lihat Daftar Buku" << endl;
        cout << "7. Lihat Daftar Peminjaman" << endl;
        cout << "8. Keluar" << endl;
        cout << "============================" << endl;

        cout << "Masukkan pilihan Anda: ";
        cin >> menu;
        system("cls");

        switch (menu) {
        case 1:
            tambahBuku();
            _getch();
            system("cls");
            break;
        case 2:
            hapusBuku();
            _getch();
            system("cls");
            break;
        case 3:
            halamanCari();
            _getch();
            system("cls");
            break;
        case 4:
            enqueuePinjam();
            _getch();
            system("cls");
            // Implementasi fungsi pinjam buku
            break;
        case 5:
            enqueueKembali();
            _getch();
            system("cls");
            break;
        case 6:
            menampilkanDaftarBuku();
            _getch();
            system("cls");
            break;
        case 7:
            daftarpeminjamanbuku();
            _getch();
            system("cls");
            break;
        case 8:
            cout << "Terima kasih! Keluar dari program." << endl;
            break;
        default:
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }

    } while (menu != 8);

    // Hapus semua node di akhir program
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }

    return 0;
}