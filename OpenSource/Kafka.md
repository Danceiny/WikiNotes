# Apache Kafka
下一代分布式消息系统。

# 基本概念
话题（Topic）是特定类型的消息流。消息是字节的有效负载（Payload），话题是消息的分类名或种子（Feed）名。
生产者（Producer）是能够发布消息到话题的任何对象。
已发布的消息保存在一组服务器中，它们被称为代理（Broker）或Kafka集群。
消费者可以订阅一个或多个话题，并从Broker拉数据，从而消费这些已发布的消息。

## Example
>Example application based on Apache Kafka framework to show it usage as distributed message server. Exploring this sample application help users to understand how good and easy is Apache Kafka usage. http://kafka.apache.org/

https://github.com/abhioncbr/Kafka-Message-Server