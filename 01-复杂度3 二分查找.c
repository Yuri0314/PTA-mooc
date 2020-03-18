Position BinarySearch( List L, ElementType X )
{
    Position left = 1, right = L->Last;
    while ( left <= right ) {
        Position mid = (left + right) >> 1;
        if ( L->Data[mid] > X )
            right = mid - 1;
        else if ( L->Data[mid] < X )
            left = mid + 1;
        else
            return mid;
    }
    return NotFound;
}