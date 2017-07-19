select
	item1,
	item2,
	item3
from
	( select
		item11,
		item12,
		item13
	  from
		schema2.table2
	  where
		idd = 2
	)
where
	id = 1
group by
	sortkey1,
	sortkey2,
	sortkye3
