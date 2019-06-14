package.json是CommonJS规定的用来描述包的文件，完全符合规范的package.json文件应该含有以下字段。
□name：包的名称，必须是唯一的，由小写英文字母、数字和下划线组成，不能包含空格。
□description：包的简要说明。
□version：符合语义化版本识别【4】规范的版本字符串。
□keywords：关键字数组，通常用于搜索。
□maintainers：维护者数组，每个元素要包含name、email（可选）、web（可选）字段。
□contributors：贡献者数组，格式与maintainers相同。包的作者应该是贡献者数组的第一个元素。
□bugs：提交bug的地址，可以是网址或者电子邮件地址。
□licenses：许可证数组，每个元素要包含type（许可证的名称）和url（链接到许可证文本的地址）字段。
□repositories：仓库托管地址数组，每个元素要包含type（仓库的类型，如git）、url（仓库的地址）和path（相对于仓库的路径，可选）字段。
□dependencies：包的依赖，一个关联数组，由包名称和版本号组成

