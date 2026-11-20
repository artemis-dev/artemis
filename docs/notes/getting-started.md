# artemis 事始め

## 要件

artemis は ROOT を用いた解析フレームワークです。楽をすること（=同じコードを書かない、無駄に手を動かさない）を目指して、個人的に開発が進められたものです。
そのため、使い始めるまでの準備は多少ステップを踏みますが、それはご容赦ください。

artemis を使うためにはいくつかの最低限の要件があります。

- git (オンライン解析時は実務上必須)
- cmake 3.31.5 以降は大丈夫（それより前でも大丈夫なこともある）
- C++17
- ROOT6 (最新版を推奨)
- yaml-cpp 0.6.3 (これ以前でもこれ以降でもなく)
- bash または zsh

さらに、連続読み出しのオンラインモニターのための zmq と redis++, hiredis のライブラリ、OpenMP（並列計算）のライブラリなどを使いたくなるかもしれません。
インストールについては README を参考に進めていただければと思います。

このページでは、artemis のバイナリおよび関連ライブラリへのパスが通って artemis が起動可能になってから、解析をどう進めるかについて記述します。

## 解析ディレクトリの準備

解析の際には様々な実験の解析に取り組んだり、オンライ解析時には複数名で解析に取り組んだりして、コードはある程度共有したいが、瞬間瞬間にはディレクトリを分けておきたいというケースがあります。そのための解析環境の構築をまずは行います。

### art_analysis のダウンロード

[githubのリポジトリ](https://github.com/artemis-dev/art_analysis)から、お好きなディレクトリへクローンしてください。ダウンロードして展開してもよいです。
同じアカウントで複数のオンライン実験を行う場合には exp/e609/ などの下に作成することをお勧めします。が、先に $HOME/art_analysis として作ってしまっていても、環境変数を使って $HOME/exp/e609/art_anslysis などにも移動できますので、まずは好きなところに配置してください。

::: warning
このディレクトリに格納されているスクリプトは bash または zsh での動作を確認しています。tcsh や csh では正しく動作しませんので、注意してください。
:::

例えば、exp の下に以下のようなディレクトリを作成してもいいですし、f77 をホームディレクトリと見立てて配置してもよいでしょう。このあと、art_analysis/.artlogin.sh を使った個人ごとの解析ディレクトリの作成の仕方を解説しますが、このスクリプトを使うと、repos/artemis-workdir-f77.git を元とする解析ディレクトリが user/[account] にクローンされて、解析環境が準備されることになります。

```
f77
├── art_analysis
│   ├── .artlogin.sh
│   ├── .git
│   ├── .gitignore
│   ├── README.md
│   └── user
└── repos
    └── artemis-workdir-f77.git
```

## artlogin コマンドの準備

art_analysis を配置できたら、次は解析前に行うオマジナイを設定します。ここでは .bashrc や .zshrc などの環境設定ファイルを使用します。

::: info 使い方
artlogin [account]
:::

とすると、[account]のディレクトリが art_analysis/user 以下に準備されるように設定をしましょう。
その際にいくつか便利な仕様になるように、artemis 本体とは無関係な設定も含まれています。
ここでは $HOME/art_analysis にクローンされており、repos/artemis-workdir.git が存在するとします。

::: details artlogin コマンドの例

```shell [zsh, bash]

function artlogin ()
{
    export ART_ANALYSIS_DIR=$HOME/art_analysis
    export ART_USER_REPOS=$HOME/repos/artemis-workdir.git
    # データディレクトリの場所(解析ディレクトリに rawdata としてリンクが作成されます)
    export ART_DATA_DIR=$HOME/data 
    . $ART_ANALYSIS_DIR/.artlogin.sh $1
    success=$?
    if [ $success = 1 ]; then
        return
    fi
 
　　 # 解析の出力ディレクトリの作成

    export ARTEMIS_OUTPUTDIR=/np1a/phanes/work/honor/f77/$ARTEMIS_USER 
    mkdir $ARTEMIS_OUTPUTDIR
    outdir='output'
    if [ ! -L $outdir ]; then
        ln -s $ARTEMIS_OUTPUTDIR $outdir
    fi

    # 以上の部分は短に mkdir output などでも良いです。
    current_shell=$(echo `basename "$SHELL"`)
    if [ $current_shell = "zsh" ]; then 
       export HISTFILE=$ARTEMIS_WORKDIR/.zsh_history_${ARTEMIS_USER}
    elif [ $current_shell = "bash" ]; then
       export HISTFILE=$ARTEMIS_WORKDIR/.bash_history_${ARTEMIS_USER}
    fi
    export HISTSIZE=100000
    export HISTFILESIZE=200000

    if [ -f CMakeLists.txt ]; then
        echo "compiling the src with cmake"
        cmake -B .build -S .
        cmake_success=$?
        cmake --build .build
        build_success=$?
        if [ ! $cmake_success = 0 ] || [ ! $build_success = 0 ]; then
            echo ""
            echo "CAUTION : Compiling src failed, please check your src is latest or not modified"
            echo ""
        fi
    fi

    echo ""
    echo "[artemis directory setting for d202407a]"
    echo ""
    env  | grep ART | column -t -s "=" | sort
    echo ""
    echo "<aliaes>"
    echo " acd : to return $ARTEMIS_WORKDIR" # これは .artlogin.sh で定義されるのでいつでも使用可能です
    echo " e   : my emacs client for $ARTEMIS_USER (now)" # これはご自身で設定した場合のみ有効です
    echo " t   : my tmux session for $ARTEMIS_USER (now)" # これはご自身で設定した場合のみ有効です
    echo ""
    echo "<CAUTION>"
    echo " dt2dl parameter should be set by using scripts/setbldprm.sh, for example "
    echo " to use 000222 in prm/mwdc/vdcgr/dt2dl/000222"
    echo " sh scripts/setbldprm.sh vdc_gr 000222"
}
```

:::

実際に artlogin してみるとどうなるでしょうか？

初めて artlogin した場合には、user アカウント名と、フルネームおよび email アドレスが質問されます。フルネームとemailアドレスは git のユーザー名になりますので必ずご自身の情報を入れるようにしてください。特に github アカウントをお持ちの方の場合は、email アドレスで紐付けられますので、紐付けられたほうが良いアカウントを選んで email アドレスを記入してください。

::: details  artlogin してみた

```
user 'online' not found.
create new user? (y/n): y
Cloning into '/home/honor/exp/f77/art_analysis/user/online'...
done.
git: 'flow' is not a git command. See 'git --help'.

The most similar commands are
 reflog
 show
input fullname: Shinsuke Ota
OK? (y/n): y
input email address: ota@rcnp.osaka-u.ac.jp
OK? (y/n): y
symbolic link ridf to directory /np1a/phanes/data/honor/f77/ created
compiling sources ../home/honor/exp/f77/art_analysis/.artlogin.sh:pushd:121: no such file or directory: src
failed
please check src/compile.log

new user 'online' created.
If you want to use some editor other than vi, set core.editor in git configuration.

```

:::

## 解析を実行する

artemis は「ステアリングファイル」と呼ばれる、解析の一連の流れを記述したファイルを理解して、解析フローを構築、実行します。
ステアリングファイルや YAML フォーマットで記述されており、リストされたプロセッサーを順番に実行していきます。
プロセッサーは単機能であることが多く、様々な検出器の信号に応用可能なものになっています。

解析ではありませんが、動作する簡単なステアリングファイルを紹介します。このファイルは artemis の example ディレクトリにありますので実行可能です。

ステアリングファイルが準備できたあと、解析を実行する3つの手順です
- artemis を起動する
- ステアリングファイルを登録する (add [steering file] [macro定義])
- 解析を実行する (resume)

::: details artemis の example を実行してみる

``` shell
# ここでは $HOME/repos/artemis/ にソースが展開されていて、
# artemis のバイナリおよびライブラリへのパスが通っていると仮定します。
> cd $HOME/repos/example 
> artemis -l 
# 色々と表示されます。
artemis [0] add rndmEventStore.tmpl.yaml 0100
# 色々と表示されます
artemis [1] resume
# しばらく時間がかかります。が次のコマンドが実行可能です。
artemis [2] ls

 artemis
>   0 TDirectoryFile top             hoge      
    1 art::TAnalysisInfo analysisInfo         

artemis [3] cd top
artemis [4] ls

 top
>   0 TH1F value           value     

artemis [5] ht 0
# X がフォワードされていたらキャンバスが表示されます。
# 表示されない場合はウェブブラウザでのアクセスが便利です。
# サーバーのポートが他の人と衝突していないかに注意してください。
# アクセス可能かどうかは計算機の管理者に相談してください。
artemis [6] new THttpServer(":5955") # JSROOT でアクセス可能になります
# tree ファイルにアクセスする（まずはファイルの存在を確認）
artemis [7] fls
 files
    0 TFile tree0100.root              (CREATE)
artemis [8] fcd 0
artemis [9] ls

 tree0100.root
>   0 art::TAnalysisInfo analysisInfo              
    1 art::TArtTree tree            tree      

artemis [10] br
random               art::TSimpleData    
rndm2                art::TSimpleData    
artemis [11] br rndm2

 art::TSimpleData

 Data Members


 Methods

                   Bool_t   CheckTObjectHashConsistency                       
             TSimpleData&   operator=                                  
             TSimpleData&   operator=                                  

 See also

     art::TSimpleDataBase<double>           

```

:::

以上で、ステアリングファイルを使ってループを回してヒストグラムを表示することができました。
ここで、"add", "resume", "cd", "ls", "ht" などの「コマンド」を使いました。
artemis ではいくつかの有用なコマンドが定義されています。自作することも可能です。"help" コマンドを実行すると、使うことができるコマンドの一覧が表示されます。ファイルを消すようなコマンドはないので、気楽に試してみてください。"help"のあとにコマンド名を続けると使い方が表示されます（ただし、一部のコマンドのみ）。
詳しくはコマンド一覧を参照してください。

ここで実行したステアリングファイルを解説します。
このステアリングファイルで使用したプロセッサーとその役割を簡単にまとめます。

|プロセッサ-クラス | 役割 | 
| --- | --- | 
|art::TRandomNumberEventStore| 乱数を決められた回数発生させる | 
|art::TTimerProcessosr | 処理時間とループあたりの時間を計測する | 
| art::TOutputTreeProcessor | プロセッサーのアウトプットを TTree に詰める |
| art::TTreeProjectionProcessor | プロセッサーのアウトプットをヒストグラムに詰める | 

::: details example にあるステアリングファイル 
``` YAML
Processor:
  - name: random
    type: art::TRandomNumberEventStore
    parameter:
      MaxLoop: 10000000000
  - name: random
    type: art::TRandomNumberEventStore
    parameter:
      MaxLoop: 10000000000
      OutputCollection: rndm2
  - name: timer
    type: art::TTimerProcessor
  - name: outputtree
    type: art::TOutputTreeProcessor
    parameter:
      SplitLevel: 99
      FileName: tree@NUM@.root
  - name: proj
    type: art::TTreeProjectionProcessor
    parameter:
      FileName: histdef.yaml
      Name: proj
      Type: art::TTreeProjection
```
:::

ステアリングファイルファイルの中で、解析手順を決めているキーは Processor になります。
このキー以降に使いたいプロセッサーを並べることになります。

プロセッサーを列挙するためには
- name
- type
が必須項目になり、"parameter"については必要に応じて設定します。

art::TRandomNumberEventStore では MaxLoop パラメータを設定しており、十分たくさんの乱数が発生されるようになっています。

art::TOutputTreeProcessor のファイル名を見てください。@NUM@と2つのアットマークで囲まれたキーワードがあります。このキーワードは add する際に指定することができます。通常は KEY=value のように等号でキーと値を結びつけて指定しますが、特に NUM についてはラン番号に使われることを想定してキーなしで数字が指定された場合には NUM を置き換えるというルールになっています。

::: details ステアリングファイルの例

``` yaml
Anchor:
  - &treeout output/@NUM@/run@NUM@.tree.root
  - &histout output/@NUM@/run@NUM@.hist.root
  - &timegate [-250.,50.]

Processor:
  - name: timer
    type: art::TTimerProcessor
  - name: MyTStreamingEventStore
    type: art::v1::TStreamingEventStore
    parameter:
      EventHeaderName: eventheader  # [TString] the name of event header
      FileName: ["rawdata/@DIR@/00/run@NUM@.dat"]  # [StringVec_t] names of input files
      MaxFrames: 0  # [Int_t] maximum number of frames (no limit if 0)
      OutputTransparency: 0  # [Bool_t] Output is persistent if false (default)
      RunHeadersName: runheader  # [TString] the name of output array for run headers
      SegmentedData: segdata  # [TString] The name of output segmented data
      StartFrame: 0  # [Int_t] start point number of frame
      Verbose: 0  # [Int_t] verbose level (default 1 : non quiet)
  - name: mapper
    type: art::TMappingProcessor
    parameter:
      OutputTransparency: 1
      MapConfig: conf/mapper.conf
      InputCollection: [segdata]
  - name: gate_init
    type: art::TGateArrayInitializer
    parameter:
      OutputTransparency: 1

  - name: proc_pla_gr_att_prm_ch2ns
    type: art::TParameterArrayLoader
    parameter:
      Name:     prm_pla_gr_att_ch2ns
      Type:     art::TAffineConverter
      FileName: prm/pla/pla_gr_att_ch2ns.dat

  - name: proc_pla_gr_att_prm_ch2MeV
    type: art::TParameterArrayLoader
    parameter:
      Name:     prm_pla_gr_att_ch2MeV
      Type:     art::TAffineConverter
      FileName: prm/pla/pla_gr_att_ch2ns.dat

  - name: proc_pla_gr_att_raw
    type: art::TTimingChargeMappingProcessor
    parameter:
      CatID:              22
      ChargeType:         2
      ChargeTypeID:       0
      TimingTypeID:       0
      OutputCollection:   pla_gr_att_raw
      OutputTransparency: 0

  - name: proc_pla_gr_att_calib
    type: art::TTimingChargeCalibrationProcessor
    parameter:
      InputCollection:      pla_gr_att_raw
      TimingConverterArray: prm_pla_gr_att_ch2ns
      ChargeConverterArray: prm_pla_gr_att_ch2MeV
      OutputCollection:     pla_gr_att_cal
      OutputTransparency:   0
  - name: proc_pla_gr_att_separater
    type: art::TSeparateOutputProcessor
    parameter:
      InputCollection: pla_gr_att_cal
      OutputCollections:
        - pla_gr_att_1l
        - pla_gr_att_1r
        - pla_gr_att_2l
        - pla_gr_att_2r
        - pla_gr_att_3l
        - pla_gr_att_3r

  - name: proc_pla_gr_att_1
    type: art::TTwoSidedPlasticCalibrationProcessor
    parameter:
      InputCollection1:   pla_gr_att_1l
      InputCollection2:   pla_gr_att_1r
      OutputCollection:   pla_gr_att_1
      ValidTimeRange: [-50.,50.,0.]  # [FloatVec_t] [min,max,offset] => Tmin = offset + min etc. ignored if min == max
      OutputTransparency: 0

  - name: proc_pla_gr_att_2
    type: art::TTwoSidedPlasticCalibrationProcessor
    parameter:
      InputCollection1:   pla_gr_att_2l
      InputCollection2:   pla_gr_att_2r
      OutputCollection:   pla_gr_att_2
      ValidTimeRange: [-50.,50.,0.]  # [FloatVec_t] [min,max,offset] => Tmin = offset + min etc. ignored if min == max
      OutputTransparency: 0

  - name: proc_pla_gr_att_3
    type: art::TTwoSidedPlasticCalibrationProcessor
    parameter:
      InputCollection1:   pla_gr_att_3l
      InputCollection2:   pla_gr_att_3r
      OutputCollection:   pla_gr_att_3
      ValidTimeRange: [-50.,50.,0.]  # [FloatVec_t] [min,max,offset] => Tmin = offset + min etc. ignored if min == max
      OutputTransparency: 0
      
  - name: gate_ps1_charge
    type: art::TTreeFormulaGateProcessor
    parameter:
      Definitions: ["mps1; pla_gr_att_1.fCharge > 90"]
  - name: gate_stop_vdc_raw
    type: art::TGateStopProcessor
    parameter:
      GateName: mps1  # [TString] name of gate to test      
  - name: proc_vdc_gr_prm_geo
    type: art::TParameterLoader
    parameter:
      Name: vdc_gr
      Type: art::TMWDCParameter
      FileName: prm/vdc_gr/geometry.yaml
  - name: proc_vdc_gr_prm_geo
    type: art::TParameterLoader
    parameter:
      Name: vdc_gr_clust
      Type: art::TMWDCParameter
      FileName: prm/vdc_gr/geometry.yaml

  - name: proc_vdc_gr_prm_x1
    type: art::TParameterLoader
    parameter:
      Name: VDC_PRM_GR_X1
      Type: art::TMonotoneTableConverter
      FileName: prm/vdc_gr/dt2dl/current/x1.dat
  - name: proc_vdc_gr_prm_x2
    type: art::TParameterLoader
    parameter:
      Name: VDC_PRM_GR_X2
      Type: art::TMonotoneTableConverter
      FileName: prm/vdc_gr/dt2dl/current/x2.dat
  - name: proc_vdc_ge_prm_u1
    type: art::TParameterLoader
    parameter:
      Name: VDC_PRM_GR_U1
      Type: art::TMonotoneTableConverter
      FileName: prm/vdc_gr/dt2dl/current/u1.dat
  - name: proc_vdc_gr_prm_u2
    type: art::TParameterLoader
    parameter:
      Name: VDC_PRM_GR_U2
      Type: art::TMonotoneTableConverter
      FileName: prm/vdc_gr/dt2dl/current/u2.dat

  - name: proc_vdc_gr_str_prm_ch2ns
    type: art::TParameterArrayLoader
    parameter:
      Name:     prm_vdc_gr_ch2ns
      Type:     art::TAffineConverter
      FileName: prm/vdc_gr/vdc_gr_str.ch2ns.dat

# load & plane calib
  - name: proc_vdc_gr_raw
    type: art::TTimingChargeMappingProcessor
    parameter:
      CatID: 51
      ChargeType:         2
      ChargeTypeID:       0
      TimingTypeID:       0
      TrailingComesFirst: 0
      OutputCollection: vdc_gr_raw
      OutputTransparency: 0
  - name: gate_vdc_raw
    type: art::TTreeFormulaGateProcessor
    parameter:
      Definitions: ["vdc_gr_raw_entry; vdc_gr_raw@.GetEntriesFast() > 11"]  # [StringVec_t] gate definitions
  - name: gate_stop_vdc_raw
    type: art::TGateStopProcessor
    parameter:
      GateName: vdc_gr_raw_entry  # [TString] name of gate to test      
  - name: proc_vdc_gr_calib
    type: art::TTimingChargeCalibrationProcessor
    parameter:
      InputCollection:      vdc_gr_raw
      TimingConverterArray: prm_vdc_gr_ch2ns
      ChargeConverterArray: prm_vdc_gr_ch2ns
      OutputCollection:     vdc_gr_cal
      OutputTransparency:   0
  - name: proc_vdc_gr_subdirs
    type: art::TTimingSubtractionProcessor
    parameter:
      InputCollection: vdc_gr_cal
      MaxDetID: 800
      OutputCollection: vdc_gr_dt
      StartTimingCollection: pla_gr_att_1
      UseMultipleStartTiming: 1
      UseStartAsTimestamp: 1  # [Int_t] timestamp is start (if 1) or end (if 0)
      StartTimingDetID: 0  # [Int_t] detID of reference timing source (if -1 use any id)      
      ValidTimeRange: [-250.,50.,0.]  # [vector<Double_t>] [min,max,offset] => Tmin = offset + min etc. ignored if min == max
  - name: proc_vdc_gr_plane
    type: art::TVDCPlaneProcessor
    parameter:
      InputCollection: vdc_gr_dt
      MWDCName: vdc_gr
      SortType: 0
      ConverterList:
        - VDC_PRM_GR_X1
        - VDC_PRM_GR_U1
        - VDC_PRM_GR_X2
        - VDC_PRM_GR_U2
      OutputTransparency: 0 
      GR_OR_LAS: 0

# cluster analysis
# require at least three hits in each plane
  - name: proc_vdc_gr_cluster_x1
    type: art::TVDCClusterizationProcessor
    parameter:
      InputCollection: vdc_gr_x1
      OutputCollection: vdc_gr_clust_x1
      OutputTransparency: 0
  - name: proc_vdc_gr_cluster_x2
    type: art::TVDCClusterizationProcessor
    parameter:
      InputCollection: vdc_gr_x2
      OutputCollection: vdc_gr_clust_x2
      OutputTransparency: 0
  - name: proc_vdc_gr_cluster_u1
    type: art::TVDCClusterizationProcessor
    parameter:
      InputCollection: vdc_gr_u1
      OutputCollection: vdc_gr_clust_u1
      OutputTransparency: 0
  - name: proc_vdc_gr_cluster_u2
    type: art::TVDCClusterizationProcessor
    parameter:
      InputCollection: vdc_gr_u2
      OutputCollection: vdc_gr_clust_u2
      OutputTransparency: 0

# tracking
  - name: proc_tracking_gr
    type: art::TVDCTrackingProcessor
    parameter:
      InputCollection:
        - vdc_gr_clust_x1
        - vdc_gr_clust_u1
        - vdc_gr_clust_x2
        - vdc_gr_clust_u2
      OutputCollection: vdc_gr
      OutputTransparency: 0


# calibration processor

  - name: proc_vdc_calib_x1
    type: art::TMWDCCalibrationProcessor
    parameter:
      InputCollection: vdc_gr_x1
      TimingGate: *timegate
  - name: proc_vdc_calib_x2
    type: art::TMWDCCalibrationProcessor
    parameter:
      InputCollection: vdc_gr_x2
      TimingGate: *timegate
  - name: proc_vdc_calib_u1
    type: art::TMWDCCalibrationProcessor
    parameter:
      InputCollection: vdc_gr_u1
      TimingGate: *timegate
  - name: proc_vdc_calib_u2
    type: art::TMWDCCalibrationProcessor
    parameter:
      InputCollection: vdc_gr_u2
      TimingGate: *timegate      
  - name: MyTTreeProjectionProcessor
    type: art::TTreeProjectionProcessor
    parameter:
      FileName: hist/basic.hist.yaml  # [TString] input filename
      OutputFilename: *histout  # [TString] output filename
      OutputTransparency: 0  # [Bool_t] Output is persistent if false (default)
      Replace: ""  # [TString] Replacement of macro
      Type: art::TTreeProjection  # [TString] type(class) of parameter
      Verbose: 1  # [Int_t] verbose level (default 1 : non quiet)
  - name: MyTTreeProjectionProcessor
    type: art::TTreeProjectionProcessor
    parameter:
      FileName: hist/correlations.hist.yaml  # [TString] input filename
      OutputFilename: *histout  # [TString] output filename
      OutputTransparency: 0  # [Bool_t] Output is persistent if false (default)
      Replace: ""  # [TString] Replacement of macro
      Type: art::TTreeProjection  # [TString] type(class) of parameter
      Verbose: 1  # [Int_t] verbose level (default 1 : non quiet)
  - name: outputtree
    type: art::TOutputTreeProcessor
    parameter:
      FileName: *treeout

```

:::
