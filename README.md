# soal-shift-sisop-modul-4-D01-2021

##  Daftar Isi ##
   - [Anggota Kelompok](https://github.com/sakti4869/soal-shift-sisop-modul-4-D01-2021/blob/main/README.md#anggota-kelompok)
   - [Soal 1](https://github.com/sakti4869/soal-shift-sisop-modul-4-D01-2021/blob/main/README.md#soal-1)
      - [1a]
      - [1b]
      - [1c]
      - [1d]
      - [1e]
   - [Soal 2](https://github.com/sakti4869/soal-shift-sisop-modul-4-D01-2021/blob/main/README.md#soal-2)
      - [2a]
      - [2b]
      - [2c]
      - [2d]
      - [2e]
   - [Soal 3](https://github.com/sakti4869/soal-shift-sisop-modul-4-D01-2021/blob/main/README.md#soal-3)
      - [3a]
      - [3b]
      - [3c]
      - [3d]
      - [3e]
   - [Soal 4](https://github.com/sakti4869/soal-shift-sisop-modul-4-D01-2021/blob/main/README.md#soal-4)
      - [4a]
      - [4b]
      - [4c]
      - [4d]
      - [4e]

## Anggota Kelompok ##

Khaela Fortunela 05111940000057

M Haikal Aria Sakti 05111940000088

David Ralphwaldo M 05111940000190

## Soal 1 ##

Di suatu jurusan, terdapat admin lab baru yang super duper gabut, ia bernama Sin. Sin baru menjadi admin di lab tersebut selama 1 bulan. Selama sebulan tersebut ia bertemu orang-orang hebat di lab tersebut, salah satunya yaitu Sei. Sei dan Sin akhirnya berteman baik. Karena belakangan ini sedang ramai tentang kasus keamanan data, mereka berniat membuat filesystem dengan metode encode yang mutakhir. Berikut adalah filesystem rancangan Sin dan Sei :

**(a)**  Jika sebuah direktori dibuat dengan awalan "AtoZ_", maka direktori tersebut akan menjadi direktori ter-encode.

### Cara Pengerjaan ###
Untuk mengecek apakah suatu direktori merupakan direktori ter-encode atau tidak, kami membuat fungsi isAtoZ yang mengembalikan value boolean. True jika direktori merupakan direktori ter-encode dan False jika direktori merupakan direktori ter-decode. Direktori yang ter-encode mempunyai substring "AtoZ" pada direktorinya.

Pseudocode:
```
dir = "/home/user/Downloads/AtoZ_folder"
if isAtoZ(dir)
  // True
else
  // False
  ```
Direktori yang baru saja dibuat adalah kosong sehingga program tidak perlu melakukan apapun.

**(b)**  Jika sebuah direktori di-rename dengan awalan "AtoZ_", maka direktori tersebut akan menjadi direktori ter-encode.

### Cara Pengerjaan ###
Ketika suatu direktori atau file di-rename, fungsi fuse ``xmp_rename`` akan terpanggil. Dengan menggunakan fungsi ini, kita dapat melakukan pengecekan apakah suatu direktori diubah menjadi direktori ter-encode atau ter-decode.

Pada fungsi ``xmp_rename``, kami mendeklarasikan string ``fpath`` dan ``tpath`` yang merupakan direktori sebelum rename dan direktori sesudah rename. Dengan kata lain, nama folder lama dan nama folder baru.

Jika ``fpath`` mempunyai substring "AtoZ_", maka ``fpath`` merupakan direktori ter-encode. Jika tidak, maka ``fpath`` merupakan direktori ter-decode. Untuk mengecek apakah ``fpath`` merupakan direktori ter-encode atau ter-decode, fungsi ``isAtoZ(fpath)`` digunakan.

Jika ``tpath`` mempunyai substring "AtoZ_", maka ``tpath`` merupakan direktori ter-encode. Jika tidak, maka ``tpath`` merupakan direktori ter-decode. Untuk mengecek apakah ``tpath`` merupakan direktori ter-encode atau ter-decode, fungsi ``isAtoZ(tpath)`` digunakan.

Untuk mengecek apakah suatu direktori sedang di-encode atau di-decode. Perbandingan return value dari fungsi ``isAtoZ(fpath)`` dan ``isAtoZ(tpath)`` digunakan. Misalkan:
```
A = isAtoZ(fpath)
B = isAtoZ(tpath)
```
Jika A = true dan B = false, maka proses decode dilakukan, sedangkan jika A = false dan B = true, maka proses encode dilakukan.
```
if A and !B
  decode()
else if !A and B
  encode()
```
**(c)**  Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.

### Cara Pengerjaan ###
Penjelasan terkait strategi penyelesaian poin c telah dijelaskan pada poin b

**(d)** Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : /home/[USER]/Downloads[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori]

### Cara Pengerjaan ###
Untuk melakukan log, prosedur logEncode dibuat. Prosedur ini berfungsi untuk mencatat suatu kegiatan ke sebuah log. Prosedur ini terpanggil sebelum proses decode atau encode selesai.
```
A = isAtoZ(fpath)
B = isAtoZ(tpath)
if A and !B
  logEncode(fpath, tpath)
  decode()
else if !A and B
  logEncode(fpath, tpath)
  encode()
  ```
Selain itu, prosedur ``logEncode`` dipanggil ketika sebuah folder dengan direktori yang mempunyai substring "AtoZ" dibuat. Untuk melakukan hal ini, fungsi fuse ``xmp_mkdir ``digunakan. Fungsi ini akan terpanggil ketika sebuah folder dibuat pada file system. Pada fungsi ini.
```
xmp_mkdir(path, ...)
if isAtoZ(path)
  logEncode("", path)
  ```
**(e)**  Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya. (rekursif)

### Cara Pengerjaan ###
Untuk melakukan encode pada suatu direktori secara rekursif hingga folder terdalam, fungsi-fungsi dari library ``dirent.h`` digunakan. Sebuah fungsi ``encodeFolderRecursively ``dan ``decodeFolderRecursively`` dibuat untuk melakukan encode dan decode pada suatu folder. Return value dari fungsi-fungsi tersebut adalah banyaknya file yang telah berhasil di encode/decode. Jika return value dari fungsi adalah nol, maka tidak ada file yang berhasil di decode/encode.

Di dalam fungsi ``encodeFolderRecursively`` dan ``decodeFolderRecursively``, proses scan file dan folder di dalamnya dilakukan. Jika sebuah folder ditemukan, maka proses decode/encode dilanjutkan pada folder tersebut.

## Soal 2 ##

Selain itu Sei mengusulkan untuk membuat metode enkripsi tambahan agar data pada komputer mereka semakin aman. Berikut rancangan metode enkripsi tambahan yang dirancang oleh Sei

a. Jika sebuah direktori dibuat dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai kasus nomor 1 dengan algoritma tambahan ROT13 (Atbash + ROT13).

Untuk menyelesaikan poin a, pertama kita buat fungsi untuk mengecek apakah direktori tersebut diawali dengan 'RX_' seperti berikut.
```
bool isRX(const char *path) {
    int len = strlen(path);
    if(path[0] == 'R' && path[1] == 'X' && path[2] == '_') return true;
    else return false;
}
```

Selanjutnya, kita buat metode enkripsi Atbash dan ROT13.
```
void encodeAtbash(char *s) {
    for (int i = 0; i < strlen(s); i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = 'A' + 'Z' - s[i] ;
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = 'a' + 'z' - s[i];
    }
}

void encodeROT13(char *s) {
    for (int i = 0; s[i]; i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = ((s[i] - 'A' + 13) % 26) + 'A';
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = ((s[i] - 'a' + 13) % 26) + 'a';
    }
}
```

b. Jika sebuah direktori di-rename dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai dengan kasus nomor 1 dengan algoritma tambahan Vigenere Cipher dengan key “SISOP” (Case-sensitive, Atbash + Vigenere).

c. Apabila direktori yang terencode di-rename (Dihilangkan “RX_” nya), maka folder menjadi tidak terencode dan isi direktori tersebut akan terdecode berdasar nama aslinya.

d. Setiap pembuatan direktori terencode (mkdir atau rename) akan tercatat ke sebuah log file beserta methodnya (apakah itu mkdir atau rename).

e. Pada metode enkripsi ini, file-file pada direktori asli akan menjadi terpecah menjadi file-file kecil sebesar 1024 bytes, sementara jika diakses melalui filesystem rancangan Sin dan Sei akan menjadi normal. Sebagai contoh, Suatu_File.txt berukuran 3 kiloBytes pada directory asli akan menjadi 3 file kecil yakni:

Suatu_File.txt.0000
Suatu_File.txt.0001
Suatu_File.txt.0002

Ketika diakses melalui filesystem hanya akan muncul Suatu_File.txt

## Soal 3 ##

Karena Sin masih super duper gabut akhirnya dia menambahkan sebuah fitur lagi pada filesystem mereka. 

a. Jika sebuah direktori dibuat dengan awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.

b. Jika sebuah direktori di-rename dengan memberi awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.

c. Apabila direktori yang terenkripsi di-rename dengan menghapus “A_is_a_” pada bagian awal nama folder maka direktori tersebut menjadi direktori normal.

d. Direktori spesial adalah direktori yang mengembalikan enkripsi/encoding pada direktori “AtoZ_” maupun “RX_” namun masing-masing aturan mereka tetap berjalan pada direktori di dalamnya (sifat recursive  “AtoZ_” dan “RX_” tetap berjalan pada subdirektori).

e. Pada direktori spesial semua nama file (tidak termasuk ekstensi) pada fuse akan berubah menjadi lowercase insensitive dan diberi ekstensi baru berupa nilai desimal dari binner perbedaan namanya.

Contohnya jika pada direktori asli nama filenya adalah “FiLe_CoNtoH.txt” maka pada fuse akan menjadi “file_contoh.txt.1321”. 1321 berasal dari biner 10100101001.

## Soal 4 ##

Untuk memudahkan dalam memonitor kegiatan pada filesystem mereka Sin dan Sei membuat sebuah log system dengan spesifikasi sebagai berikut.

a. Log system yang akan terbentuk bernama “SinSeiFS.log” pada direktori home pengguna (/home/[user]/SinSeiFS.log). Log system ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.

b. Karena Sin dan Sei suka kerapian maka log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.

c. Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.

d. Sisanya, akan dicatat pada level INFO.

e. Format untuk logging yaitu:


[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]

Level : Level logging, dd : 2 digit tanggal, mm : 2 digit bulan, yyyy : 4 digit tahun, HH : 2 digit jam (format 24 Jam),MM : 2 digit menit, SS : 2 digit detik, CMD : System Call yang terpanggil, DESC : informasi dan parameter tambahan

INFO::28052021-10:00:00:CREATE::/test.txt
INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt


