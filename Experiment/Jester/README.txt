*******************************************************

注释：以下说明以Jester_User-num-15000_Item-2文件夹内容为例，其他几个文件同理。

*******************************************************

程序名称：Borda's-Count_Kendall's-Tau
1.输入文件：input_Jester-1-user_num-15000_item-2
2.输出文件：Output_Borda's-Count-Kendall's-Tau_User'num-15000_item-2
3.程序作用：计算不加隐私的博尔达计数排序及平均肯德尔距离

*******************************************************

程序名称：Naive-RAPPOR
1.输入文件：input_all-rank-of-2和input_Jester-1-user_num-15000_item-2
2.输出文件：Output_Naive-RAPPOR_User'num-15000_item-2_Epsilon-0.01_for-1000
3.程序作用：使用Naive-RAPPOR（可能项目数量N！）算法完成对数据差分隐私保护，循环执行1000次，
记录每一的实验结果，并综合1000次实验结果给出了最大肯德尔距离，最小肯德尔距离、平均肯德尔距离
及可能项目统计结果（清零与不清零），以及MAE值。item及Epsilon参数进行更改。
名词解释：
Bef-correction-Statistics：校正前统计结果
Aft-correction-Statistics：校正后统计结果
Cl0-correction-Statistics：清零后统计结果
max_a_kendall：最大肯德尔距离(带有Clear的是清零后的结果)
min_a_kendall：最小肯德尔距离(带有Clear的是清零后的结果)
all_average_kendall：平均肯德尔距离(带有Clear的是清零后的结果)
average_tongji：1000次循环的平均统计结果(带有Clear的是清零后的结果)
YS_TJ_GS：真实数据统计结果
average_tongji_zh：转换成与Straightforward-RAPPOR相对应的统计形式的隐私保护统计结果(带有Clear的是清零后的结果)
YS_TJ_GS_zh：转换成与Straightforward-RAPPOR相对应的统计形式的真实统计结果
MAE：MAE值(带有Clear的是清零后的结果)

*******************************************************

程序名称：Straightforward-RAPPOR
1.输入文件：input_Jester-1-user_num-15000_item-2
2.输出文件：Output_Straightforward-RAPPOR_User'num-15000_item-2_Epsilon-0.01_for-1000
3.程序作用：使用Straightforward-RAPPOR（可能项目数量N*N）算法完成对数据差分隐私保护，循环执行1000次，
记录每一的实验结果，并综合1000次实验结果给出了最大肯德尔距离，最小肯德尔距离、平均肯德尔距离
及可能项目统计结果（清零与不清零），以及MAE值。item及Epsilon参数进行更改。
名词解释：
Bef-correction-Statistics：校正前统计结果
Aft-correction-Statistics：校正后统计结果
Cl0-correction-Statistics：清零后统计结果
max_a_kendall：最大肯德尔距离(带有Clear的是清零后的结果)
min_a_kendall：最小肯德尔距离(带有Clear的是清零后的结果)
all_average_kendall：平均肯德尔距离(带有Clear的是清零后的结果)
average_tongji：1000次循环的平均统计结果(带有Clear的是清零后的结果)
YS_TJ_GS：真实数据统计结果
MAE：MAE值(带有Clear的是清零后的结果)

*******************************************************
