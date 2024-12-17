# Huffman_zip
Compress and uncompress the file using Huffman encoding

## resource 
本项目是基于川大数据结构实验二，使用哈夫曼编码方式压缩文件和解压文件

## function
当前项目仅仅支持纯英文的文本文件、视频和图像。

### compress
压缩文件，生成三个目标文件，一个是哈夫曼树结构文件，一个是子节点信息文件，还有压缩后的.zip文件（这个.zip文件不能正常解压，只是形式上采用的.zip文件格式表示为压缩后的文件）。
实测Oxford_English_Dictionary_(Oxford_English_Dictionary)_(Z-Library).txt文件压缩率可达50%，针对纯英文文本效率挺高的。

### uncompress
解压文件，生成源内容文件，会默认在同目录下寻找相关的两个配置文件，没有找到则需要指定文件地址。
