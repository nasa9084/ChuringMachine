#TuringMachine
プログラムとテープをtxtファイルで読み込んで動作するチューリングマシンです。

$ gcc -lm TuringMachine.c  
$./a.out Program.txt Tape.txt  

で動作させます。

プログラムの形式は、

[記号の数],[状態の数]  
<記号のリスト>  
カンマ区切りプログラム  

となっています。カンマの前後には空白を入れないでください。  
テープの形式は、

テープの長さ(出力後の長さも見据えて、大きめに)  
テープの内容  

となっています。
