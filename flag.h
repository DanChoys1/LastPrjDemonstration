#ifndef FLAG_H
#define FLAG_H

/// Флаг проверки значения.
/// После проверки меняет свое значение на true или false
class Flag
{
public:
    enum Type
    {
        IsChecked,      // Флаг будет возвращать true если флаг был проверен
        IsNotChecked    // Флаг будет возвращать true если флаг еще не был проверен
    };

    Flag(Type t = IsChecked)
    {
        _flagCheckedReturn = t == IsChecked;
    }

    operator bool()
    {
        return check();
    }

    bool check()
    {
        if (!_isChecked)
        {
            _isChecked = true;
            return !_flagCheckedReturn;
        }

        return _flagCheckedReturn;
    }

    void reset()
    {
        _isChecked = false;
    }

private:
    bool _flagCheckedReturn = false;
    bool _isChecked = false;

};

#endif // FLAG_H
