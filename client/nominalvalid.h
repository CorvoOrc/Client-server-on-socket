/* Steshenko A.S. IVT-265 */

#ifndef NOMINALVALID_H
#define NOMINALVALID_H

#include <QValidator>
#include <QString>

class NominalValid : public QValidator
{
	Q_OBJECT

public:
	NominalValid(QObject *parent);
	NominalValid();
	~NominalValid();
	QValidator::State validate(QString & input, int & pos ) const;

private:
	
};

#endif // NOMINALVALID_H
