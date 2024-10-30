
/**
 * @file
 * @brief 素数を用いた転置式暗号を提供するライブラリです。
 */

#pragma once
#include <stddef.h>
#include <stdint.h>

/**
 * @brief 各種暗号関数で利用される鍵素数を保存するための型です。
 */

typedef uint64_t simple_pcypher_key;

/**
 * @brief simple_pcypher_key 型の最大値です。
 */

#define SIMPLE_PCYPHER_KEY_MAX UINT64_MAX

/**
 * @brief 各種暗号関数が失敗した際に設定される値が定義された列挙型です。
 * @param SIMPLE_PCYPHER_ERRNO_NONE 関数は失敗しなかった。
 * @param SIMPLE_PCYPHER_ERRNO_COULD_NOT_FIND_BEST_PRIME_NUMBER 最適な素数を見つけることができなかった。
 * @param SIMPLE_PCYPHER_ERRNO_KEY_AND_BLOCK_ARE_OVERSIZE 鍵素数と暗号ブロックの大きさの積が SIZE_MAX を超過した。
 * @param SIMPLE_PCYPHER_ERRNO_INDEX_OUT_OF_RANGE データの読み出し位置が、データの末尾を超過した。
 */

typedef enum _simple_pcypher_errno {
  SIMPLE_PCYPHER_ERRNO_NONE = 0,
  SIMPLE_PCYPHER_ERRNO_COULD_NOT_FIND_BEST_PRIME_NUMBER,
  SIMPLE_PCYPHER_ERRNO_KEY_AND_BLOCK_ARE_OVERSIZE,
  SIMPLE_PCYPHER_ERRNO_INDEX_OUT_OF_RANGE,
} _simple_pcypher_errno;

/**
 * @brief 各種暗号関数が失敗した際に設定される値が格納される大域変数です。
 * @note この大域変数は errno と同様にスレッドローカルに保存されます。
 */

extern thread_local _simple_pcypher_errno simple_pcypher_errno;

/**
 * @brief simple_pcypher_errno の値をメッセージ表示に適した文字列に変換します。
 * @param errno simple_pcypher_errno に設定された値です。
 * @return errno の内容にあった静的な文字列を返します。
 * @note 未定義の値が errno に指定された場合、この関数は `"Unknown errno given."` を返します。
 */

extern char __stdcall *simple_xcypher_errno_message (_simple_xcypher_errno errno);

/**
 * @brief 暗号化されたデータを保存する領域の大きさを求めます。
 * @param datasize 暗号化するデータの大きさです。
 * @param encrypteddatasizep 計算された領域の大きさが書き込まれる size_t 型のポインタです。
 * @return 成功ならば 0 失敗ならば 1 を返します。
 * @note 計算された領域の大きさは datasize 以上の値をもつ素数になります。そのような素数を SIZE_MAX 未満で見つけることができなかった場合、この関数は失敗します。
 */

extern int __stdcall simple_pcypher_calc_encrypted_data_size (size_t datasize, size_t *encrypteddatasizep);

/**
 * @brief 各種暗号関数で利用される鍵素数を求めます。
 * @param keybase 鍵素数の基となる整数値です。
 * @param encrypteddatasize simple_pcypher_calc_encrypted_data_size() 関数で得られた暗号化されたデータを保存する領域の大きさです。
 * @param keyp 計算された鍵素数が書き込まれる simple_pcypher_key 型のポインタです。
 * @return 成功ならば 0 失敗ならば 1 を返します。
 * @note 計算された鍵素数の値は n != encrypteddatasize かつ n * encrypteddatasize <= SIZE_MAX の条件を満たす素数になります。そのため encrypteddatasize の値があまりにも大きい場合には keybase から離れた値が返される場合があります。
 * @warning encrypteddatasize にあまりにも大きい値を指定すると、対となる素数を見つけることができずに関数が失敗する恐れがあります。
 */

extern int __stdcall simple_pcypher_calc_key (simple_pcypher_key keybase, size_t encrypteddatasize, simple_pcypher_key *keyp);

/**
 * @brief 鍵素数を用いてデータを暗号化します。
 * @param data 暗号化するデータのポインタです。
 * @param datasize 暗号化するデータの大きさです。
 * @param key 暗号化に使われる素数です。
 * @param encrypteddata 暗号化されたデータが書き込まれる領域のポインタです。
 * @param encrypteddatasize 暗号化されたデータが書き込まれる領域の大きさです。
 * @return 成功ならば 0 失敗ならば 1 を返します。
 * @note key は simple_pcypher_calc_key() 関数で計算された素数を指定することを強く推奨します。
 * @note encrypteddata, encrypteddatasize はそれぞれ simple_pcypher_calc_encrypted_data_size() 関数で計算された素数の大きさ、値を指定することを強く推奨します。
 * @warning key, encrypteddatasize にあまりにも大きい値を指定すると、計算時の桁あふれを防止するために、関数が失敗する恐れがあります。
 */

extern int __stdcall simple_pcypher_encrypt (const void *data, size_t datasize, simple_pcypher_key key, void *encrypteddata, size_t encrypteddatasize);

/**
 * @brief 鍵素数を用いてデータを部分的に復号します。
 * @param position 復号を開始する、平文を基準とした位置です。
 * @param size 復号を行う大きさです。
 * @param data 復号するデータのポインタです。
 * @param datasize 復号するデータの大きさです。
 * @param key 復号に使われる素数です。
 * @param decrypteddata 復号されたデータが書き込まれる領域のポインタです。
 * @return 成功ならば 0 失敗ならば 1 を返します。
 * @note key は simple_pcypher_calc_key() 関数で計算された素数を指定することを強く推奨します。
 * @warning decrypteddata の領域の大きさは size 以上でなければなりません。
 * @warning position, size の合計が datasize を超過する場合、メモリを保護するために、この関数は失敗します。
 */

extern int __stdcall simple_pcypher_decrypt (size_t position, size_t size, const void *data, size_t datasize, simple_pcypher_key key, void *decrypteddata);
