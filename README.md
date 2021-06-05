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

a. Jika sebuah direktori dibuat dengan awalan "AtoZ_", maka direktori tersebut akan menjadi direktori ter-encode.

b. Jika sebuah direktori di-rename dengan awalan "AtoZ_", maka direktori tersebut akan menjadi direktori ter-encode.

c. Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.

d. Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : /home/[USER]/Downloads[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori]

e. Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya. (rekursif)

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


