- 有用的函数：filter_var

```php
<?php
$reqBody = '{"username":"hz"}';
$context = stream_context_create(array(
'http'=>array('method'=>'POST','header'=>'Content-Type: application/json;charset=utf-8;\r\n' . 'Content-Length: ' . mb_strlen($reqBody),'conetent'=>$reqBody)));
$resp = file_get_contents('https://api.cannot.cc/users', false, $context);

// 某些文件系统函数如file(),fpassthru()只能用php://filter流封装协议来附加流过滤器。
$handle = fopen('php://filter/read=string.toupper/resource=data.txt', 'rb');
while(feof($handle)!==true){
    echo fgets($handle);//输出大写字母
}

```



- 自定义流过滤器

```php
class DirtyWordsFilter extends php_user_filter{
    public function filter($in, $out, &$consumed, $closing){
        $words = array('grime', 'dirt', 'grease');
        $wordData = array();
        foreach($words as $word){
            $replacement = array_fill(0, mb_strlen($word), '*')
            $wordData[$word] = implode('', $replacement);
        }
        $bad = array_keys($wordData);
        $good = array_values($wordData);
        // 迭代流来的桶队列中的每个桶
    	while($bucket=stream_bucket_make_writeable($in)){
            // 审查桶数据中的脏字
            $bucket->data = str_replace($bad, $good, $bucket->data);
            $consumed += $bucket->datalen;
            stream_bucket_append($out, $bucket);
        }
    }
}
stream_filter_register('dirty_words_filter', 'DirtyWordsFilter');
```

