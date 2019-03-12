The Apriori algorithm works in two steps:
1. Generate all frequent itemsets:
频次集的事务支持超过最小支持？
???
>A frequent itemset is an itemset that has transaction support above minsup.

2. Generate all confident association rules from the frequent itemsets:
置信联系规则的置信概率应超过最小置信概率？
>A confident association rule is a rule with confidence above minconf.


# Frequent Itemset Generation
**Downward Closure Property**
>If an itemset has minimum support, then
every non-empty subset of this itemset also has minimum support.
