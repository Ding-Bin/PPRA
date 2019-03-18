***************************************************

程序名称：Jester_dataset_filter__jester-data-1_Number-of-occurrences_user-num-24983_item-100
1.输入文件：input_jester-data-1
2.输出文件：output_jester-data-1-result_Number-of-occurrences
3.程序作用：jester-data-1数据集是24983个评论人针对100个笑话进行打分，分数范围为-10至+10,99代表
没有对该笑话评论。本程序是针对这一数据及统计出每一个笑话被评论的个数，并按照有大到小的顺序进行排序。

***************************************************

程序名称：Jester_dataset_filter__jester-data-1_Removed-99_user-num-24983_item-100
1.输入文件：input_jester-data-1_Top-10     （10可替换为20/30/40/50/60）
2.输出文件：output_jester-data-1_Top-10_no-99     （10可替换为20/30/40/50/60）
3.程序作用：输入文件是从jester-data-1数据集中筛选出来的评论个数前10、前20，....，前60的项目，
这其中包含99（即没有评论该笑话），本程序的作用是筛选出对top项目内的所有项目都评论的用户，
只要存在没有评论的就把该用户剔除。

***************************************************

程序名称：Jester_dataset_filter__jester-data-1_TOP-item-select_user-num-15000_item-60
1.输入文件：input_Top-60-user_num-15000
2.输出文件：output_Top-2-user_num-15000       （2可替换为3/4/5/.../60）
3.程序作用：输入文件是15000名用户评论top60项目数据，本程序作用是在这样一个群体中筛选出top2/3/4/
/.../60个项目的数据。

***************************************************

程序名称：Jester_dataset_filter__jester-data-1_goal-to-rank_user-num-15000_item-60
1.输入文件：input_Top-60-user_num-15000
2.输出文件：output_XuHao_Top-2
3.程序作用：本程序作用是将分数转换为排序。

***************************************************
