###Trade
####OrderInsert
struct CQTFtdcInputOrderField
| field_name | type | description |
| ------ | ------ | ------|
| OrderSysID | char[] | System assign value |
| ParticipantID | char[] | such as 000000001 |
| ClientID | char[] | such as 0000000023 |
| UserID | char[] | [seat_id] such as 900000029 |
| InstrumentID | char[] | such as CYB-BTC |
| OrderPriceType | char | ['1']:random_price ['2']:limit_price ['3']:optimal_price ['4']:fivelevel_price |
| Direction | char | ['0']:buy ['1']:sell |
| CombOffsetFlag | char[] | ['0']:open ['1']:close|
| CombHedgeFlag | char[] | ['0']: |
| LimitPrice | double | price such as 5.000 |
| VolumeTotalOriginal | long long | Volume such as 1000000000|
| TimeCondition |char | valid_period------['1']:(IOC,execute immediately otherwise withdraw) ['2']:(GFS,valid in this period) ['3']:(GFD,valid today) ['4']:(GTD,valid before the appointed day) ['5']:(GTC,valid before withdrawing) ['6']:(GFA,valid in assemble price)|
| GTDDate | char[] | a date type|
| VolumeCondition | char | a volume denoting trade_volume,['1']:(AV,any volume) ['2']:(MV,minmum volume) ['3']:(CV,all volume)|
| MinVolume | long long | minimum trade volume |
| ContingentCondition | char | a trrigercondition: ['1']:immediately triggered ['2']:stop loss triggered |
| StopPrice | double | stop loss price |
| ForceCloseReason | char[] | ['0']:not force close ['1']:lack of Deposit ['2']:client's position is beyond the position_limit ['3']:the member's position is beyond the position limit ['4']:the positon is not multiple ['5']:violation ['6']:other reason ['7']:person is near the delivery |
| OrderLocalID | char[] | order local id|
| IsAutoSuspend | int | a flag denoting whether autosuspended or not |
| BusinessUnit | char[] | a business unit|

