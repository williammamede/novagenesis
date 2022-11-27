import { Component, OnInit } from '@angular/core';
import { MatDialog } from '@angular/material/dialog';
import { Messages } from './data-monitor.model';
import { DataMonitorService } from './data-monitor.service';
import { DataViewerDialogComponent } from './data-viewer-dialog/data-viewer-dialog.component';

@Component({
  selector: 'app-data-monitor',
  templateUrl: './data-monitor.component.html',
  styleUrls: ['./data-monitor.component.css']
})
export class DataMonitorComponent implements OnInit {

  constructor(private dataMonitorService: DataMonitorService, public dialog: MatDialog) { }

  protected offeredServices: string[] = [];
  protected transferedData: Messages[] = [];
  protected receivedData: Messages[] = [];
  displayedColumns: string[] = ['fileName', 'time', 'source', 'size'];

  ngOnInit(): void {
    setInterval(() => {
      this.setOfferedServices();
      this.setTransferedData();
      this.setReceivedData();
    }, 2000);
  }

  setOfferedServices() {
    this.dataMonitorService.getOfferedServices().subscribe(data => {
      this.offeredServices = data.ServiceOffer;
    });
  }

  setTransferedData() {
    this.dataMonitorService.getTransferedData().subscribe(data => {
      this.transferedData = data.PublishedMessages;
    });
  }

  setReceivedData() {
    this.dataMonitorService.getReceivedData().subscribe(data => {
      this.receivedData = data.ReceivedMessages;
    });
  }

  showData(data: Messages) {
    const dialogRef = this.dialog.open(DataViewerDialogComponent, {
      width: '80%',
      height: '80%',
      data: data
    });
  }

}
