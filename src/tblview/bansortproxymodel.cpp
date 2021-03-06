
#include "includes.hpp"
#include "bansortproxymodel.hpp"

BanSortProxyModel::BanSortProxyModel()
{

}

BanSortProxyModel::~BanSortProxyModel()
{

}

bool BanSortProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
    int column = sortColumn();
    if ( column >= 0 )
    {
        QVariant vL = sourceModel()->data( left );
        QVariant vR = sourceModel()->data( right );

        if ( !( vL.isValid() && vR.isValid() ))
            return QSortFilterProxyModel::lessThan( left, right );

        Q_ASSERT( vL.isValid( ) );
        Q_ASSERT( vR.isValid( ) );

        QString vlStr{ vL.toString() };
        QString vrStr{ vR.toString() };

        if ( column == BanWidget::dv
          || column == BanWidget::wv )
        {
            vlStr = QString::number( vlStr.toUInt( 0, 16 ) );
            vrStr = QString::number( vrStr.toUInt( 0, 16 ) );
        }
        else if ( column == BanWidget::date )
        {
            vlStr = QString::number(
                        QDateTime::fromString( vlStr,
                                               "ddd MMM dd HH:mm:ss yyyy" )
                             .toTime_t() );

            vrStr = QString::number(
                        QDateTime::fromString( vrStr,
                                               "ddd MMM dd HH:mm:ss yyyy" )
                             .toTime_t() );
        }
        else if ( column == BanWidget::sn )
        {
            if ( !vlStr.contains( "SOUL" ) )
                vlStr = Helper::intSToStr( vlStr, 10 );
            else
                vlStr = vlStr.remove( "SOUL", Qt::CaseInsensitive ).trimmed();

            if ( !vrStr.contains( "SOUL" ) )
                vrStr = Helper::intSToStr( vrStr, 10 );
            else
                vrStr = vrStr.remove( "SOUL", Qt::CaseInsensitive ).trimmed();

            vlStr = Helper::intSToStr( vlStr, 10 );
            vrStr = Helper::intSToStr( vrStr, 10 );
        }

        bool res = false;
        if ( Helper::naturalSort( vlStr, vrStr, res ) )
            return res;
    }
    return QSortFilterProxyModel::lessThan( left, right );
}
