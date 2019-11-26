#include "OperatorsModel.h"
#include "OperatorsDelegate.h"
#include "OperatorsView.h"

#include <QApplication>
#include <QLoggingCategory>
#include <QFile>

#include <EditorWidget.h>

namespace L {
Q_LOGGING_CATEGORY(main, "main", QtWarningMsg);
}

using namespace mobile;

class Foo{};

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	auto dbPath = QCoreApplication::applicationDirPath() + "/system.db";
	if (!QFile(dbPath).exists()) {
		qCWarning(L::main) << "Database file does not exists";
		return -1;
	}

	auto db = std::make_shared<Storage>(dbPath, "QSQLITE");
	if (!db.get()->open()) {
		return -1;
	}

	OperatorsModel model(db);
	OperatorsDelegate delegate;
	OperatorsView view(db);
	view.setModel(&model);
	view.setItemDelegate(&delegate);

	view.setHeaderHidden(true);
	view.setWindowTitle("Mobile operators");
	view.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
	view.show();

	return app.exec();
}
