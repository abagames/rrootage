rRootage  readme.txt
for Windows98/2000/XP(要OpenGL)
ver. 0.23
(C) Kenta Cho

自動生成ボスシューティング、rRootage。


○ インストール方法

rr0_23.zipを適当なフォルダに展開してください。
その後、'rr.exe'を実行してください。
（マシンの速度が遅い場合は、'rr_lowres.bat'を実行してください。
  rRootageを低解像度で立ち上げます。）


○ 遊び方

 - 移動         矢印キー / ジョイステック
 - レーザー     [Z]      / トリガ1, トリガ4
 - スペシャル   [X]      / トリガ2, トリガ3
 - ポーズ       [P]

キーボードかジョイスティックでステージを選んでください。
レーザーキーでゲームを開始します。

また、4つのゲームモードを選ぶことができます。
スペシャルキーを押して、ゲームモードを変更してください。

. Normal mode
 基本的なゲームモードです。
 レーザーキーを押している間、自機は遅くなります。
 - スペシャル -> ボム
 ボムは敵弾を消し去ります。
 ボムの残弾数は右下に表示されています。

. PSY mode
 自機が敵弾にかすることに、右下に表示されているグレイズメーターが 
 増加します。グレイズメーターがフルになると、一定時間自機が
 無敵になります。
 - スペシャル -> ローリング
 この動作を行うことで、自機がかすることができる範囲が増加します。
 スペシャルキーを押している間、自機は遅くなります。
 速く動きたい場合は、スペシャルキーを軽くたたくようにしてください。

. IKA mode
 自機が白と黒の2つの属性を持ちます。
 すべての敵弾も同様の属性を持ち、 自機は同じ属性の敵弾を
 吸収することができます。吸収された敵弾は、自動的にレーザーに
 変換されます。
 - スペシャル -> 属性変更
 自機の属性を変更します。

. GW mode
 自機がリフレクターを利用できます。
 リフレクターは周辺の敵弾を跳ね返します。
 - スペシャル -> リフレクター
 リフレクターを使うには、スペシャルキーを右下のリフレクターメーターが
 空になるまで押し続けてください。リフレクターはリフレクターメーターに
 'OK'の表示があるときのみ使えます。

自機を操作して、弾幕を避け、レーザーでボスを破壊してください。
レーザーは、ボスに接近して使用するほど、よりダメージを与えます。

自機がすべて破壊されると、ゲームオーバーです。
自機は200,000点および500,000点ごとに1機増えます。

以下のオプションが指定できます。
 -lowres        低解像度モードを利用します。
 -nosound       音を出力しません。
 -window        ウィンドウモードで起動します。
 -reverse       レーザーとボムのキーを入れ替えます。


○ ご意見、ご感想

コメントなどは、cs8k-cyu@asahi-net.or.jp までお願いします。


○ ウェブページ

rRootage webpage:
http://www.asahi-net.or.jp/~cs8k-cyu/windows/rr.html


○ 謝辞

BulletMLファイルのパースにlibBulletMLを利用しています。
 libBulletML
 http://user.ecc.u-tokyo.ac.jp/~s31552/wp/libbulletml/

画面の出力にはSimple DirectMedia Layerを利用しています。
 Simple DirectMedia Layer
 http://www.libsdl.org/

BGMとSEの出力にSDL_mixerとOgg Vorbis CODECを利用しています。
 SDL_mixer 1.2
 http://www.libsdl.org/projects/SDL_mixer/
 Vorbis.com
 http://www.vorbis.com/

乱数発生器にMersenne Twisterを利用しています。
 http://www.math.keio.ac.jp/matumoto/emt.html


○ ヒストリ

2003  8/15  ver. 0.23
            弾幕の調整。
            libBulletMLのアップデート。
            メモリ関連バグ修正。
2003  4/26  ver. 0.22
            弾幕の調整。
2003  4/ 4  ver. 0.21
            弾幕の追加。
            弾幕の調整。
2003  3/21  ver. 0.2
            ゲームモードを追加。
            タイマがボスの形態変化時に止まるように。
            弾幕の調整。
2003  3/16  ver. 0.1
            初公開版。


○ ライセンス

rRootageはBSDスタイルライセンスのもと配布されます。

License
-------

Copyright 2003 Kenta Cho. All rights reserved. 

Redistribution and use in source and binary forms, 
with or without modification, are permitted provided that 
the following conditions are met: 

 1. Redistributions of source code must retain the above copyright notice, 
    this list of conditions and the following disclaimer. 

 2. Redistributions in binary form must reproduce the above copyright notice, 
    this list of conditions and the following disclaimer in the documentation 
    and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
