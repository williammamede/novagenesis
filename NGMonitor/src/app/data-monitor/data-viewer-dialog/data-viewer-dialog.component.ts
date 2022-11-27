import { Component, Inject } from '@angular/core';
import { MAT_DIALOG_DATA } from '@angular/material/dialog';
import { PublishedMessages } from '../data-monitor.model';

@Component({
    selector: 'app-data-viewer-dialog',
    templateUrl: './data-viewer-dialog.component.html'
})
export class DataViewerDialogComponent {

    constructor(@Inject(MAT_DIALOG_DATA) public data: PublishedMessages) { }

    ngOnInit(): void {
    }

}