/**
 * @file   TAffineConverterArrayGenerator.h
 * @brief  generates array of TAffineConverter
 *
 * @date   Created:       2013-10-14 15:18:39
 *         Last Modified: 2014-01-10 14:28:03
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

/* # about Parameter
 *
 * parameter:
 *   # 出力とパラメータファイルのpathを並べたもの。下の例のように複数指定可能
 *   - name:  ParameterList
 *     value: ["para1", "path/to/file1",
 *             "para2", "path/to/file2",
 *              ...
 *             "paraN", "path/to/fileN"]
 *
 * # format of parameter file
 *
 *   - 一列にa,bをスペース区切りで併記する
 *   - 2つの数字以降の記述は全て無視されるのでコメントを記述することも可能
 *   
 *   ex.1) 100 0.25
 *          -> 100 + 0.25 x
 *   ex.2) 10 0.001 # hogehoge
 *          -> 10 + 0.001 x
 *
 *   - 数字2つで行が始まらない場合、その行は無視される
 *   - 生成される配列の要素数はファイルのうち有効な行数となる
 *
 */

#ifndef TAFFINECONVERTERARRAYGENERATOR_H
#define TAFFINECONVERTERARRAYGENERATOR_H

#include <TProcessor.h>

namespace art{
   class TAffineConverterArrayGenerator;
}

class TList;

class art::TAffineConverterArrayGenerator : public TProcessor {
public:
   // Default constructor
   TAffineConverterArrayGenerator();
   virtual ~TAffineConverterArrayGenerator();

   virtual void Init(TEventCollection *col);

protected:
   TList        *fConverterArrayList; // generated arrays are owned by this list
   StringVec_t   fParameterList;
private:
   // Copy constructor (prohibited)
   TAffineConverterArrayGenerator(const TAffineConverterArrayGenerator& rhs);
   // Assignment operator (prohibited)
   TAffineConverterArrayGenerator& operator=(const TAffineConverterArrayGenerator& rhs);

   ClassDef(TAffineConverterArrayGenerator,0) // generator of array of affine converter
};

#endif // TAFFINECONVERSIONARRAYGENERATOR_H
