select  t1.id,  t1.time as "100",   t2.time as "200",   t3.time as "300",   t4.time as "400",   t5.time as "500", 
                t6.time as "600",   t7.time as "700",   t8.time as "800",   t9.time as "900",   t10.time as "1000", 
                t11.time as "1100", t12.time as "1200", t13.time as "1300", t14.time as "1400", t15.time as "1500",
                t16.time as "1600", t17.time as "1700", t18.time as "1800", t19.time as "1900"
from timing_100 as t1
    join timing_200 as t2 on t1.id = t2.id      join timing_300 as t3 on t2.id = t3.id 
    join timing_400 as t4 on t3.id = t4.id      join timing_500 as t5 on t4.id = t5.id     join timing_600 as t6 on t5.id = t6.id 
    join timing_700 as t7 on t6.id = t7.id      join timing_800 as t8 on t7.id = t8.id     join timing_900 as t9 on t8.id = t9.id
    join timing_1000 as t10 on t9.id = t10.id   join timing_1100 as t11 on t10.id = t11.id join timing_1200 as t12 on t11.id = t12.id
    join timing_1300 as t13 on t12.id = t13.id  join timing_1400 as t14 on t13.id = t14.id join timing_1500 as t15 on t14.id = t15.id 
    join timing_1600 as t16 on t15.id = t16.id  join timing_1700 as t17 on t16.id = t17.id join timing_1800 as t18 on t17.id = t18.id 
    join timing_1900 as t19 on t18.id = t19.id  join timing_2000 as t20 on t19.id = t20.id