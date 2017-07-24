/*
    Took from qt source for usage in old qt versions
*/
#  define VERIFY_EXCEPTION_THROWN(expression, exceptiontype) \
    do {\
        QT_TRY {\
            QT_TRY {\
                expression;\
                QTest::qFail("Expected exception of type " #exceptiontype " to be thrown" \
                             " but no exception caught", __FILE__, __LINE__);\
                return;\
            } QT_CATCH (const exceptiontype &) {\
            }\
        } QT_CATCH (const std::exception &e) {\
            QByteArray msg = QByteArray() + "Expected exception of type " #exceptiontype \
                             " to be thrown but std::exception caught with message: " + e.what(); \
            QTest::qFail(msg.constData(), __FILE__, __LINE__);\
            return;\
        } QT_CATCH (...) {\
            QTest::qFail("Expected exception of type " #exceptiontype " to be thrown" \
                         " but unknown exception caught", __FILE__, __LINE__);\
            return;\
        }\
    } while (0)