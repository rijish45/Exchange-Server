CFLAGS=-std=c++14 -pedantic -Wall -Werror -O3
# SRCS= $(filter-out create.cpp, $(wildcard *.cpp))
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))

server: $(OBJS)
	g++ -o $@ $(OBJS) -lpq -lpthread  -lpqxx -lpq

%.o:%.cpp
	g++ $(CFLAGS) -c $<

.PHONY: clean depend
clean:
	rm -f processes server *.o *~ *\#

depend:
	makedepend $(SRCS)
# DO NOT DELETE

socketException.o: socketException.hpp myException.hpp
socket.o: socket.hpp /usr/include/unistd.h /usr/include/features.h
socket.o: /usr/include/stdc-predef.h /usr/include/netdb.h
socket.o: /usr/include/netinet/in.h /usr/include/endian.h
socket.o: /usr/include/rpc/netdb.h socketException.hpp myException.hpp
myException.o: myException.hpp
transaction.o: transaction.hpp pugixml.hpp pugiconfig.hpp pugixml.cpp
transaction.o: pugixml.hpp /usr/include/stdlib.h /usr/include/stdio.h
transaction.o: /usr/include/string.h /usr/include/assert.h
transaction.o: /usr/include/features.h /usr/include/stdc-predef.h
transaction.o: /usr/include/limits.h /usr/include/math.h
transaction.o: /usr/include/stdint.h
connectionPool.o: connectionPool.hpp /usr/include/pqxx/pqxx
connectionPool.o: /usr/include/pqxx/binarystring
connectionPool.o: /usr/include/pqxx/binarystring.hxx
connectionPool.o: /usr/include/pqxx/compiler-public.hxx
connectionPool.o: /usr/include/pqxx/config-public-compiler.h
connectionPool.o: /usr/include/pqxx/compiler-internal-pre.hxx
connectionPool.o: /usr/include/pqxx/result /usr/include/pqxx/result.hxx
connectionPool.o: /usr/include/pqxx/internal/result_data.hxx
connectionPool.o: /usr/include/pqxx/internal/libpq-forward.hxx
connectionPool.o: /usr/include/pqxx/except /usr/include/pqxx/except.hxx
connectionPool.o: /usr/include/pqxx/util /usr/include/pqxx/util.hxx
connectionPool.o: /usr/include/pqxx/strconv /usr/include/pqxx/strconv.hxx
connectionPool.o: /usr/include/pqxx/compiler-internal-post.hxx
connectionPool.o: /usr/include/pqxx/field /usr/include/pqxx/field.hxx
connectionPool.o: /usr/include/pqxx/tuple /usr/include/pqxx/tuple.hxx
connectionPool.o: /usr/include/pqxx/connection
connectionPool.o: /usr/include/pqxx/connection.hxx
connectionPool.o: /usr/include/pqxx/connectionpolicy
connectionPool.o: /usr/include/pqxx/connectionpolicy.hxx
connectionPool.o: /usr/include/pqxx/basic_connection
connectionPool.o: /usr/include/pqxx/basic_connection.hxx
connectionPool.o: /usr/include/pqxx/connection_base
connectionPool.o: /usr/include/pqxx/connection_base.hxx
connectionPool.o: /usr/include/pqxx/errorhandler
connectionPool.o: /usr/include/pqxx/errorhandler.hxx
connectionPool.o: /usr/include/pqxx/prepared_statement
connectionPool.o: /usr/include/pqxx/prepared_statement.hxx
connectionPool.o: /usr/include/pqxx/internal/statement_parameters.hxx
connectionPool.o: /usr/include/pqxx/cursor /usr/include/pqxx/cursor.hxx
connectionPool.o: /usr/include/pqxx/transaction_base
connectionPool.o: /usr/include/pqxx/transaction_base.hxx
connectionPool.o: /usr/include/pqxx/isolation /usr/include/pqxx/isolation.hxx
connectionPool.o: /usr/include/pqxx/largeobject
connectionPool.o: /usr/include/pqxx/largeobject.hxx
connectionPool.o: /usr/include/pqxx/dbtransaction
connectionPool.o: /usr/include/pqxx/dbtransaction.hxx
connectionPool.o: /usr/include/pqxx/nontransaction
connectionPool.o: /usr/include/pqxx/nontransaction.hxx
connectionPool.o: /usr/include/pqxx/notification
connectionPool.o: /usr/include/pqxx/notification.hxx
connectionPool.o: /usr/include/pqxx/notify-listen
connectionPool.o: /usr/include/pqxx/notify-listen.hxx
connectionPool.o: /usr/include/pqxx/pipeline /usr/include/pqxx/pipeline.hxx
connectionPool.o: /usr/include/pqxx/robusttransaction
connectionPool.o: /usr/include/pqxx/robusttransaction.hxx
connectionPool.o: /usr/include/pqxx/subtransaction
connectionPool.o: /usr/include/pqxx/subtransaction.hxx
connectionPool.o: /usr/include/pqxx/tablereader
connectionPool.o: /usr/include/pqxx/tablereader.hxx
connectionPool.o: /usr/include/pqxx/tablestream
connectionPool.o: /usr/include/pqxx/tablestream.hxx
connectionPool.o: /usr/include/pqxx/tablewriter
connectionPool.o: /usr/include/pqxx/tablewriter.hxx
connectionPool.o: /usr/include/pqxx/transaction
connectionPool.o: /usr/include/pqxx/transaction.hxx
connectionPool.o: /usr/include/pqxx/transactor
connectionPool.o: /usr/include/pqxx/transactor.hxx /usr/include/pqxx/version
connectionPool.o: /usr/include/pqxx/version.hxx /usr/include/assert.h
connectionPool.o: /usr/include/features.h /usr/include/stdc-predef.h
pugixml.o: pugixml.hpp pugiconfig.hpp /usr/include/stdlib.h
pugixml.o: /usr/include/stdio.h /usr/include/string.h /usr/include/assert.h
pugixml.o: /usr/include/features.h /usr/include/stdc-predef.h
pugixml.o: /usr/include/limits.h /usr/include/math.h /usr/include/stdint.h
processes.o: processes.hpp /usr/include/pqxx/pqxx
processes.o: /usr/include/pqxx/binarystring
processes.o: /usr/include/pqxx/binarystring.hxx
processes.o: /usr/include/pqxx/compiler-public.hxx
processes.o: /usr/include/pqxx/config-public-compiler.h
processes.o: /usr/include/pqxx/compiler-internal-pre.hxx
processes.o: /usr/include/pqxx/result /usr/include/pqxx/result.hxx
processes.o: /usr/include/pqxx/internal/result_data.hxx
processes.o: /usr/include/pqxx/internal/libpq-forward.hxx
processes.o: /usr/include/pqxx/except /usr/include/pqxx/except.hxx
processes.o: /usr/include/pqxx/util /usr/include/pqxx/util.hxx
processes.o: /usr/include/pqxx/strconv /usr/include/pqxx/strconv.hxx
processes.o: /usr/include/pqxx/compiler-internal-post.hxx
processes.o: /usr/include/pqxx/field /usr/include/pqxx/field.hxx
processes.o: /usr/include/pqxx/tuple /usr/include/pqxx/tuple.hxx
processes.o: /usr/include/pqxx/connection /usr/include/pqxx/connection.hxx
processes.o: /usr/include/pqxx/connectionpolicy
processes.o: /usr/include/pqxx/connectionpolicy.hxx
processes.o: /usr/include/pqxx/basic_connection
processes.o: /usr/include/pqxx/basic_connection.hxx
processes.o: /usr/include/pqxx/connection_base
processes.o: /usr/include/pqxx/connection_base.hxx
processes.o: /usr/include/pqxx/errorhandler
processes.o: /usr/include/pqxx/errorhandler.hxx
processes.o: /usr/include/pqxx/prepared_statement
processes.o: /usr/include/pqxx/prepared_statement.hxx
processes.o: /usr/include/pqxx/internal/statement_parameters.hxx
processes.o: /usr/include/pqxx/cursor /usr/include/pqxx/cursor.hxx
processes.o: /usr/include/pqxx/transaction_base
processes.o: /usr/include/pqxx/transaction_base.hxx
processes.o: /usr/include/pqxx/isolation /usr/include/pqxx/isolation.hxx
processes.o: /usr/include/pqxx/largeobject /usr/include/pqxx/largeobject.hxx
processes.o: /usr/include/pqxx/dbtransaction
processes.o: /usr/include/pqxx/dbtransaction.hxx
processes.o: /usr/include/pqxx/nontransaction
processes.o: /usr/include/pqxx/nontransaction.hxx
processes.o: /usr/include/pqxx/notification
processes.o: /usr/include/pqxx/notification.hxx
processes.o: /usr/include/pqxx/notify-listen
processes.o: /usr/include/pqxx/notify-listen.hxx /usr/include/pqxx/pipeline
processes.o: /usr/include/pqxx/pipeline.hxx
processes.o: /usr/include/pqxx/robusttransaction
processes.o: /usr/include/pqxx/robusttransaction.hxx
processes.o: /usr/include/pqxx/subtransaction
processes.o: /usr/include/pqxx/subtransaction.hxx
processes.o: /usr/include/pqxx/tablereader /usr/include/pqxx/tablereader.hxx
processes.o: /usr/include/pqxx/tablestream /usr/include/pqxx/tablestream.hxx
processes.o: /usr/include/pqxx/tablewriter /usr/include/pqxx/tablewriter.hxx
processes.o: /usr/include/pqxx/transaction /usr/include/pqxx/transaction.hxx
processes.o: /usr/include/pqxx/transactor /usr/include/pqxx/transactor.hxx
processes.o: /usr/include/pqxx/version /usr/include/pqxx/version.hxx
processes.o: transaction.hpp pugixml.hpp pugiconfig.hpp pugixml.cpp
processes.o: pugixml.hpp /usr/include/stdlib.h /usr/include/stdio.h
processes.o: /usr/include/string.h /usr/include/assert.h
processes.o: /usr/include/features.h /usr/include/stdc-predef.h
processes.o: /usr/include/limits.h /usr/include/math.h /usr/include/stdint.h
processes.o: connectionPool.hpp socket.hpp /usr/include/unistd.h
processes.o: /usr/include/netdb.h /usr/include/netinet/in.h
processes.o: /usr/include/endian.h /usr/include/rpc/netdb.h
processes.o: socketException.hpp myException.hpp
main.o: socket.hpp /usr/include/unistd.h /usr/include/features.h
main.o: /usr/include/stdc-predef.h /usr/include/netdb.h
main.o: /usr/include/netinet/in.h /usr/include/endian.h
main.o: /usr/include/rpc/netdb.h socketException.hpp myException.hpp
main.o: transaction.hpp pugixml.hpp pugiconfig.hpp pugixml.cpp pugixml.hpp
main.o: /usr/include/stdlib.h /usr/include/stdio.h /usr/include/string.h
main.o: /usr/include/assert.h /usr/include/limits.h /usr/include/math.h
main.o: /usr/include/stdint.h processes.hpp /usr/include/pqxx/pqxx
main.o: /usr/include/pqxx/binarystring /usr/include/pqxx/binarystring.hxx
main.o: /usr/include/pqxx/compiler-public.hxx
main.o: /usr/include/pqxx/config-public-compiler.h
main.o: /usr/include/pqxx/compiler-internal-pre.hxx /usr/include/pqxx/result
main.o: /usr/include/pqxx/result.hxx
main.o: /usr/include/pqxx/internal/result_data.hxx
main.o: /usr/include/pqxx/internal/libpq-forward.hxx /usr/include/pqxx/except
main.o: /usr/include/pqxx/except.hxx /usr/include/pqxx/util
main.o: /usr/include/pqxx/util.hxx /usr/include/pqxx/strconv
main.o: /usr/include/pqxx/strconv.hxx
main.o: /usr/include/pqxx/compiler-internal-post.hxx /usr/include/pqxx/field
main.o: /usr/include/pqxx/field.hxx /usr/include/pqxx/tuple
main.o: /usr/include/pqxx/tuple.hxx /usr/include/pqxx/connection
main.o: /usr/include/pqxx/connection.hxx /usr/include/pqxx/connectionpolicy
main.o: /usr/include/pqxx/connectionpolicy.hxx
main.o: /usr/include/pqxx/basic_connection
main.o: /usr/include/pqxx/basic_connection.hxx
main.o: /usr/include/pqxx/connection_base
main.o: /usr/include/pqxx/connection_base.hxx /usr/include/pqxx/errorhandler
main.o: /usr/include/pqxx/errorhandler.hxx
main.o: /usr/include/pqxx/prepared_statement
main.o: /usr/include/pqxx/prepared_statement.hxx
main.o: /usr/include/pqxx/internal/statement_parameters.hxx
main.o: /usr/include/pqxx/cursor /usr/include/pqxx/cursor.hxx
main.o: /usr/include/pqxx/transaction_base
main.o: /usr/include/pqxx/transaction_base.hxx /usr/include/pqxx/isolation
main.o: /usr/include/pqxx/isolation.hxx /usr/include/pqxx/largeobject
main.o: /usr/include/pqxx/largeobject.hxx /usr/include/pqxx/dbtransaction
main.o: /usr/include/pqxx/dbtransaction.hxx /usr/include/pqxx/nontransaction
main.o: /usr/include/pqxx/nontransaction.hxx /usr/include/pqxx/notification
main.o: /usr/include/pqxx/notification.hxx /usr/include/pqxx/notify-listen
main.o: /usr/include/pqxx/notify-listen.hxx /usr/include/pqxx/pipeline
main.o: /usr/include/pqxx/pipeline.hxx /usr/include/pqxx/robusttransaction
main.o: /usr/include/pqxx/robusttransaction.hxx
main.o: /usr/include/pqxx/subtransaction /usr/include/pqxx/subtransaction.hxx
main.o: /usr/include/pqxx/tablereader /usr/include/pqxx/tablereader.hxx
main.o: /usr/include/pqxx/tablestream /usr/include/pqxx/tablestream.hxx
main.o: /usr/include/pqxx/tablewriter /usr/include/pqxx/tablewriter.hxx
main.o: /usr/include/pqxx/transaction /usr/include/pqxx/transaction.hxx
main.o: /usr/include/pqxx/transactor /usr/include/pqxx/transactor.hxx
main.o: /usr/include/pqxx/version /usr/include/pqxx/version.hxx
main.o: connectionPool.hpp
