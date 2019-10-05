/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author @todo Add author
 *
 * @brief @todo Add brief description
 **/

#ifndef ARINC665_QT_MEDIASETDIALOG_HPP
#define ARINC665_QT_MEDIASETDIALOG_HPP

#include <arinc665_qt/Arinc665Qt.hpp>

#include <QDialog>

#include <memory>

namespace Arinc665Qt{

namespace Ui{
class MediaSetDialog;
}

class MediaSetDialog : public QDialog
{
  Q_OBJECT

  public:
    /**
     * @brief Initialises the ASF Messages dialog.
     *
     * @param[in] parent
     *   Widget parent.
     **/
    explicit MediaSetDialog( QWidget *parent = nullptr);

    //! Destructor
    ~MediaSetDialog() override;

    /**
     * @brief Sets the Model of the Dialog.
     *
     * @param[in] mediaSetModel
     *   The Media Set Model.
     **/
    void model( MediaSetModel * mediaSetModel);

  private slots:

  private:
    //! UI (designer)
    std::unique_ptr< Ui::MediaSetDialog> ui;
    //! Media Set Model
    MediaSetModel * mediaSetModelV;
};

}

#endif
