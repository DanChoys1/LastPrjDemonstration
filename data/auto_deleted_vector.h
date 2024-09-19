#ifndef AUTODELETEDVECTOR_H
#define AUTODELETEDVECTOR_H

#include <QQueue>
#include <QTime>
#include <QTimer>
#include <QVector>

/// Вектор который удаляет сам удаляет свои элементы с задержкаой delay.
/// Работает только с одним методом append(const T& val), т.к. перегружен только он!!!!
/// Время определяется через функцию QTime::currTime(),
/// возможна не корректная при изменении времени в системе
template<class T>
class AutoDeletedVector : public QVector<T>
{
public:
    AutoDeletedVector()
    {
        _checkDeletingValsTimer.setSingleShot(true);
        QObject::connect(&_checkDeletingValsTimer, &QTimer::timeout, [this](){ checkDeletingVals(); });
    }

    AutoDeletedVector(const AutoDeletedVector& other) :
        QVector<T> (other), _deleteDelay(other._deleteDelay), _appendItemTimes(other._appendItemTimes)
    {
        _checkDeletingValsTimer.setSingleShot(true);
        QObject::connect(&_checkDeletingValsTimer, &QTimer::timeout, [this](){ checkDeletingVals(); });
    }

    AutoDeletedVector<T>& operator = (const AutoDeletedVector<T>& other)
    {
        QVector<T>::operator=(other);
        _deleteDelay = other._deleteDelay;
        _appendItemTimes = other._appendItemTimes;

        return *this;
    }

    void append(const T& val)
    {
        QVector<T>::append(val);
        _appendItemTimes.enqueue(QTime::currentTime());
        checkDeletingVals();
    }

    /// Задержка удаления элементов
    void setDeleteDelay(qreal delay)
    {
        if (qFuzzyCompare(_deleteDelay, delay)) // Чтобы лишний раз не вызывать checkPoint();
            return;

        _deleteDelay = delay;
        checkDeletingVals();
    }

private:
    /// Проверка есть ли элементы на удаление
    void checkDeletingVals()
    {
        if (_appendItemTimes.isEmpty())
            return;

        // Время прошедшее с добавления первого элемента массива
        qreal lostTime = _appendItemTimes.head().secsTo(QTime::currentTime());
        // Удаление элементов время которых вышло
        while (lostTime > _deleteDelay)
        {
            // Удаление элемента и массива
            this->takeFirst();
            // Удаление времени добавленя элемента в массив
            _appendItemTimes.dequeue();

            // Если больше элементов нет, возврат
            if (_appendItemTimes.isEmpty())
                return;

            // Время прошедшее с добавления первого элемента массива
            lostTime = _appendItemTimes.head().secsTo(QTime::currentTime());
        }

        // Запустить таймер с задержкой до следующего времени удаления элемента
        _checkDeletingValsTimer.start((_deleteDelay - lostTime) * 1000);
    }

private:
    /// Задержка удаления элементов
    qreal _deleteDelay = 0;
    /// Времена добавления элементов
    QQueue<QTime> _appendItemTimes;
    /// Таймер проверки элементов на удаление
    QTimer _checkDeletingValsTimer;

};


#endif // AUTODELETEDVECTOR_H
