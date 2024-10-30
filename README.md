
# simple-pcypher

![](https://img.shields.io/badge/-C++-blue?logo=cplusplus)
![](https://img.shields.io/badge/license-MIT-blue)
![](https://img.shields.io/badge/version-1.0.0-orange)

simple-pcypher は素数を用いた転置式暗号を提供します。

提供される暗号機能は次のような特徴を有しています。

* ブロック暗号です。
* 共通鍵暗号方式を採用しています。
* ブロックの大きさと共通鍵に素数を用います。
* 部分的な復号が可能です(ランダムアクセス)。
* 転置式暗号なので、所定の位置に必ず出現する文章があっても、そこを足掛かりに復号されてしまう危険性が低くなります。

提供される暗号機能で留意すべき点は次のとおりです。

* コストの高い演算が行われているので、実行速度が遅いです。
* 当然ですが、これも同梱されている鍵を抜き取られてしまうと、正規の手順で復号されてしまいます。

## Details 

simple-pcypher の転置先の位置計算には次の経験則が用いられています。

```
ふたつの素数 A, B と任意の非負整数 n があるとする。
n < B かつ A != B の条件を満たすとき n * A mod B によって得られる値はそれぞれ重複しない。
```

ただし、この式をそのまま実装すると `n = 0` のときの値が `A, B` の値によらず `0` になってしまいます。
そのため、もう一工夫加えて、転置先の位置を次のように求めます。

```
転置先の位置は (n + A) mod B * A mod B で求める。
```

これにより任意の位置 `n` に対して、安全な転置先の位置を一意に求めることができるようになりました。
つまり `n, A, B` の３つが揃っていれば、転置先が常に分かるため、暗号化されたデータを部分的に復号することが可能になるという訳です。

しかし、ただ単純に転置しただけでは暗号強度が脆弱なので、実際には転置操作に加えて、各バイト毎に Xorlshift とバーナム暗号を組み合わせた暗号化が追加で施されています。

## Usage

ビルド後に作成される `dist/include` `dist/lib` 内のファイルを適切な場所に配置して使います。 それ以外は標準的なC言語ライブラリと同じように扱います。

```c
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <simple-pcypher/simple-pcypher.h>

#define PLAINTEXT "This is plaintext!"

int main (){

  size_t encrypteddatasize;
  simple_pcypher_calc_encrypted_data_size(sizeof(PLAINTEXT), &encrypteddatasize);

  simple_pcypher_key key;
  simple_pcypher_calc_key(0x123, encrypteddatasize, &key);

  uint8_t encrypteddata[encrypteddatasize];
  simple_pcypher_encrypt(PLAINTEXT, sizeof(PLAINTEXT), key, encrypteddata, encrypteddatasize);
  printf("encrypted = ");
  fwrite(encrypteddata, encrypteddatasize, 1, stdout);
  putc('\n', stdout);

  uint8_t decrypteddata[sizeof(PLAINTEXT)];
  simple_pcypher_decrypt(0, sizeof(PLAINTEXT), encrypteddata, encrypteddatasize, key, decrypteddata);
  printf("decrypted = %s\n", decrypteddata);

  return 0;

}
```

```bash
gcc -Idist/include -Ldist/lib example.c -lsimplepcypher
```

## Command 

ビルド後に作成される `bin/simple-pcypher.exe` を実行することで、コマンドラインから暗号機能を利用することができます。

```bash
./bin/simple-pcypher -k 0x123
```

## Build 

同梱されている `makefile` を使用することでビルドすることができます。

```bash
make
```

非常に簡素なテストも同梱されています。

```bash
make test
```

## License 

&copy; 2024 tikubonn

simple-pcypher released under the [MIT License](./LICENSE).
