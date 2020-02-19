## Bayan - ����� ���������� ������

**������� ������**: ��������� �������������� � ������ ����������: 
"����������� ���-�� ��������� � �����". � ������������ � �������� ����� � ���������� ����������� - ��������� ��������� �� ������� 
��������� � ����� ������� ������ ���������� ������
(��� ������ ��������� ��. ���� ������ **������**)

**�������������:**

```./bayan --help```

```./bayan --block-size <������_�����> --hash-type <crc32|md5> <����>*```

```./bayan --block-size <������_�����> --hash-type <crc32|md5> --dir <����������>```

**���������**
 - `--help` - ������� ������� ������� �� ���������

 - `--block-size <������_�����>` - ��� ������ ������, �� ������� ����������� ����� � 
������� ������������ ����������� (�.�. �������� ���������� � �����)

 - `--hash-type <crc32|md5|sha1>` - ���� �� ���������� ����������� (����� ���� ������ ������ ����)
 
 - `<����>*` - ����� ���������� ������������ ������
 
 - `--dir <����������>` - ���� ���������� ��� ����� �������, ���� �������� (���������� ��������������� ��� ��������� ��������)

��� ��������� ����� �������� �����: ��. ������� --help

**������� (������ �� ���������� ./build):**

`./Debug/bayan.exe --hash-type crc32 --block-size 5 --dir ../tests`

`./Debug/bayan.exe --hash-type crc32 --block-size 5 "../tests/trivial_test/file1.txt" "../tests/trivial_test/file2.txt" "../tests/trivial_test/file3.txt"`

**������ � ��������� �������:**

```./Debug/bayan.exe -t crc32 -s 5 -d ../tests```

**������**

 - ������ ������������ � ������� ����������� CMAKE:

```
cd <bayan_sources>
mkdir build && cd ./build
cmake ..
cmake --build .
./Debug/tests.exe
```

 - ��� ��������� ������ ����������� (�������� ��-���������) ���������� �������� ��� ������ LOGS � ������ ���������:

```
cmake -DLOGS=ON --build . && cmake --build . && ./Debug/tests.exe
```